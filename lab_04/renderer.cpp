#include "renderer.hpp"
#include <float.h>
#include <process.h>

static void swap_points(point_2d &a, point_2d &b);
static void swap_points(point_3d &a, point_3d &b);

static inline int are_equal(point_2d &a, point_2d &b);
static inline int are_equal(point_3d &a, point_3d &b); 

static void rotate_point(point_2d &point, point_2d &center, float angle);

void
FillBuffer(uint32_t *BitmapBuffer, int BitmapWidth, int BitmapHeight, uint32_t Color)
{
    for (int i = 0; i < BitmapWidth * BitmapHeight; ++i) {
        BitmapBuffer[i] = Color;
    }
}

int cmpInts(const void *a, const void *b) {
    return *(int *)a > *(int *)b;
}

void 
ZBuffer(uint32_t *BitmapBuffer, int BitmapWidth, int BitmapHeight, polygons_3d &Polygons)
{
    double Eps = 1e-8;
    // Note: If this is stack memory -> better
    double *Depth = (double *)malloc(sizeof(double) * BitmapWidth * BitmapHeight);
    for (int i = 0; i < BitmapWidth * BitmapHeight; ++i) {
        Depth[i] = DBL_MAX;
    }

    for (int iPol = 0; iPol < Polygons.n; ++iPol) {
        polygon_3d *curPolygon = &Polygons.p[iPol];

        point_3d minP = {INT_MAX, INT_MAX, INT_MAX};
        point_3d maxP = {INT_MIN, INT_MIN, INT_MIN};

        for (int iVertex = 0; iVertex < curPolygon->nVertexes; ++iVertex) {
            if (curPolygon->v[iVertex].y < minP.y)
                minP = curPolygon->v[iVertex];
            if (curPolygon->v[iVertex].y > maxP.y)
                maxP = curPolygon->v[iVertex];
        }

        for (int y = minP.y; y <= maxP.y; ++y) {
            int interPoints[255] = {};
            int count = 0;

            for (int j = 0; j < curPolygon->nVertexes; ++j) {
                int next = (j + 1) % curPolygon->nVertexes;

                if ((curPolygon->v[j].y > y && curPolygon->v[next].y <= y) || 
                    (curPolygon->v[next].y > y && curPolygon->v[j].y <= y)) {
                    interPoints[count++] = curPolygon->v[j].x + 
                                           (y - curPolygon->v[j].y) * (curPolygon->v[next].x - curPolygon->v[j].x) / 
                                           (curPolygon->v[next].y - curPolygon->v[j].y);
                }
            }

            qsort(interPoints, count, sizeof(*interPoints), cmpInts);

            for (int j = 0; j < count; j += 2) {
                for (int x = interPoints[j]; x <= interPoints[j + 1]; ++x) {
                    if (0 <= y && y < BitmapHeight && 0 <= x && x < BitmapWidth) {
                        double z = minP.z - (curPolygon->normal.x * (double)(x - minP.x) + curPolygon->normal.y * (double)(y - minP.y)) / (double)curPolygon->normal.z;

                        if (z < Depth[y * BitmapWidth + x]) {
                            Depth[y * BitmapWidth + x] = z;
                            BitmapBuffer[y * BitmapWidth + x] = curPolygon->color;
                        }
                    }
                }
            }
        }
    }

    free(Depth);
}

typedef struct {
    uint32_t *Color;
    double *Depth;

    point_2d minP;
    point_2d maxP;

    int y;

    int BitmapWidth;

    polygons_3d *Polygons;
    int startPolygon;
    int endPolygon;
} thread_data;

DWORD WINAPI ProcessPolygons(LPVOID arg) {
    thread_data *data = (thread_data *)arg;

    for (int iPol = 0; iPol < data->Polygons->n; ++iPol) {
        polygon_3d *curPolygon = &data->Polygons->p[iPol];

        point_3d minP = {INT_MAX, INT_MAX, INT_MAX};
        point_3d maxP = {INT_MIN, INT_MIN, INT_MIN};

        for (int iVertex = 0; iVertex < curPolygon->nVertexes; ++iVertex) {
            if (curPolygon->v[iVertex].y < minP.y)
                minP = curPolygon->v[iVertex];
            if (curPolygon->v[iVertex].y > maxP.y)
                maxP = curPolygon->v[iVertex];
        }

        for (int y = data->minP.y; y <= data->maxP.y; ++y) {
            int interPoints[255] = {};
            int count = 0;

            for (int j = 0; j < curPolygon->nVertexes; ++j) {
                int next = (j + 1) % curPolygon->nVertexes;

                if ((curPolygon->v[j].y > y && curPolygon->v[next].y <= y) || 
                    (curPolygon->v[next].y > y && curPolygon->v[j].y <= y)) {
                    interPoints[count++] = curPolygon->v[j].x + 
                                           (y - curPolygon->v[j].y) * (curPolygon->v[next].x - curPolygon->v[j].x) / 
                                           (curPolygon->v[next].y - curPolygon->v[j].y);
                }
            }

            qsort(interPoints, count, sizeof(*interPoints), cmpInts);

            for (int j = 0; j < count; j += 2) {
                for (int x = interPoints[j]; x <= interPoints[j + 1]; ++x) {
                    if (data->minP.y <= y && y < data->maxP.y && data->minP.x <= x && x < data->maxP.x) {
                        double z = minP.z - (curPolygon->normal.x * (double)(x - minP.x) + curPolygon->normal.y * (double)(y - minP.y)) / (double)curPolygon->normal.z;

                        if (z < data->Depth[y * data->BitmapWidth + x]) {
                            data->Depth[y * data->BitmapWidth + x] = z;
                            data->Color[y * data->BitmapWidth + x] = curPolygon->color;
                        }
                    }
                }
            }
        }
    }

    return 0;
}

void ZBufferM(uint32_t *BitmapBuffer, int BitmapWidth, int BitmapHeight, polygons_3d &Polygons, int nThreads)
{
    HANDLE *threads = (HANDLE *)malloc(nThreads * sizeof(HANDLE));
    thread_data *threadData = (thread_data *)malloc(nThreads * sizeof(thread_data));

    double *mainDepth = (double *)malloc(sizeof(double) * BitmapWidth * BitmapHeight);
    for (int i = 0; i < BitmapWidth * BitmapHeight; ++i) {
        mainDepth[i] = DBL_MAX;
    }

    int childWidth = BitmapWidth;
    int childHeight = BitmapHeight / nThreads;
    int polygonsPerThread = Polygons.n / nThreads;

    for (int i = 0; i < nThreads; ++i) {
        threadData[i].Color = BitmapBuffer;
        threadData[i].Depth = mainDepth;

        threadData[i].minP = {0, i * childHeight};
        threadData[i].maxP = {childWidth, (i + 1) * childHeight};
        threadData[i].BitmapWidth = BitmapWidth;

        threadData[i].y = i;

        threadData[i].Polygons = &Polygons;
        threadData[i].startPolygon = i * polygonsPerThread;
        threadData[i].endPolygon = (i == nThreads - 1) ? Polygons.n : (i + 1) * polygonsPerThread;

        threads[i] = CreateThread(NULL, 0, ProcessPolygons, (LPVOID)&threadData[i], 0, NULL);
    }

    WaitForMultipleObjects(nThreads, threads, TRUE, INFINITE);

    for (int i = 0; i < nThreads; ++i) {
        CloseHandle(threads[i]);
    }

    free(threads);
    free(threadData);
    free(mainDepth);
}

void 
DrawPoint2D(uint32_t *BitmapBuffer, int BitmapWidth, point_2d P, uint32_t Color) 
{
    BitmapBuffer[P.y * BitmapWidth + P.x] = Color;
}

void 
DrawLine2D(uint32_t *BitmapBuffer, int BitmapWidth, point_2d P1, point_2d P2, uint32_t Color)
{
    long dx = abs(P2.x - P1.x);
    long dy = abs(P2.y - P1.y);
    long sx = (P1.x < P2.x) ? 1 : -1;
    long sy = (P1.y < P2.y) ? 1 : -1;
    long err = dx - dy;

    while (true) {
        BitmapBuffer[P1.y * BitmapWidth + P1.x] = Color;

        if (P1.x == P2.x && P1.y == P2.y)
            break;
        
        long err2 = err * 2;
        if (err2 > -dy) {
            err -= dy;
            P1.x += sx;
        }
        if (err2 < dx) {
            err += dx;
            P1.y += sy;
        }
    }
}

void 
DrawTriangle2D(uint32_t *BitmapBuffer, int BitmapWidth, point_2d P1, point_2d P2, point_2d P3, uint32_t Color) 
{
    // TODO: Make it more efficient
    DrawLine2D(BitmapBuffer, BitmapWidth, P1, P2, Color);
    DrawLine2D(BitmapBuffer, BitmapWidth, P2, P3, Color);
    DrawLine2D(BitmapBuffer, BitmapWidth, P1, P3, Color);
}

void 
FillTriangle2D(uint32_t *BitmapBuffer, int BitmapWidth, point_2d P1, point_2d P2, point_2d P3, uint32_t Color)
{
    if (P1.y > P2.y) swap_points(P1, P2);
    if (P1.y > P3.y) swap_points(P1, P3);
    if (P2.y > P3.y) swap_points(P2, P3);

    DrawLine2D(BitmapBuffer, BitmapWidth, P1, P2, Color); // Do we really need to do it first?
    DrawLine2D(BitmapBuffer, BitmapWidth, P2, P3, Color);
    DrawLine2D(BitmapBuffer, BitmapWidth, P1, P3, Color);

    for (int y = P1.y; y <= P3.y; ++y) {
        int x1 = (y < P2.y) ? ((P1.x + (y - P1.y) * (P2.x - P1.x) / (P2.y - P1.y))) : ((P2.x + (y - P2.y) * (P3.x - P2.x) / (P3.y - P2.y)));
        int x2 = (y < P2.y) ? ((P1.x + (y - P1.y) * (P3.x - P1.x) / (P3.y - P1.y))) : ((P2.x + (y - P2.y) * (P3.x - P2.x) / (P3.y - P2.y)));
        
        for (int x = min(x1, x2); x <= max(x1, x2); ++x) {
            BitmapBuffer[y * BitmapWidth + x] = Color;
        }
    }
}

void 
FillPolygon2D(uint32_t *BitmapBuffer, int BitmapWidth, point_2d *Points, int n, uint32_t Color)
{
    int i, j, temp;

    int yMin = INT_MAX;
    int yMax = 0;

    for (i = 0; i < n; ++i) {
        if (Points[i].y < yMin)
            yMin = Points[i].y;
        if (Points[i].y > yMax)
            yMax = Points[i].y;
    }

    for (i = yMin; i <= yMax; ++i) {
        int interPoints[255] = {}; // What is wrong with this array? 255 -> n
        int count = 0;

        for (j = 0; j < n; ++j) {
            int next = (j + 1) % n;

            if ((Points[j].y > i && Points[next].y <= i) || (Points[next].y > i && Points[j].y <= i)) {
                interPoints[count++] = Points[j].x + round((float)(i - Points[j].y) * (Points[next].x - Points[j].x) / (float)(Points[next].y - Points[j].y));
            }
        }

        for (j = 0; j < count - 1; ++j) { // To make it more efficient use better sort
            for (int k = 0; k < count - j - 1; ++k) {
                if (interPoints[k] > interPoints[k + 1]) {
                    temp = interPoints[k];
                    interPoints[k] = interPoints[k + 1];
                    interPoints[k + 1] = temp;
                }
            }
        }

        for (j = 0; j < count; j += 2) {
            DrawLine2D(BitmapBuffer, BitmapWidth, {interPoints[j], i}, {interPoints[j + 1], i}, Color);
        }
    }
}

void RotatePolygons3D(polygons_3d &polygons, point_3d &rotate_center, double yaw, double pitch, double roll)
{
    for (int i = 0; i < polygons.n; ++i) {
        for (int j = 0; j < polygons.p[i].nVertexes; ++j) {
            RotatePoint3D(polygons.p[i].v[j], rotate_center, yaw, pitch, roll);
        }
    }
}

void RotatePoint3D(point_3d &point, point_3d &rotate_center, double yaw, double pitch, double roll)
{
    double x = point.x - rotate_center.x;
    double y = point.y - rotate_center.y;
    double z = point.z - rotate_center.z;

    yaw = yaw * Pi32 / 180;
    pitch = pitch * Pi32 / 180;
    roll = roll * Pi32 / 180;

    point.x = round(x * (cos(yaw) * cos(pitch)) + y * (cos(yaw) * sin(pitch) * sin(roll) - sin(yaw) * cos(roll)) +
              z * (cos(yaw) * sin(pitch) * cos(roll) + sin(yaw) * sin(roll)));
    point.y = round(x * (sin(yaw) * cos(pitch)) + y * (sin(yaw) * sin(pitch) * sin(roll) + cos(yaw) * cos(roll)) +
              z * (sin(yaw) * sin(pitch) * cos(roll) - cos(yaw) * sin(roll)));
    point.z = round(x * (-sin(pitch)) + y * (cos(pitch) * sin(roll)) + z * (cos(pitch) * cos(roll)));

    point.x += rotate_center.x;
    point.y += rotate_center.y;
    point.z += rotate_center.z;
}

static void swap_points(point_2d &a, point_2d &b)
{
    point_2d tmp = a;
    a = b;
    b = tmp;
}

static void swap_points(point_3d &a, point_3d &b)
{
    point_3d tmp = a;
    a = b;
    b = tmp;
}

static int are_equal(point_2d &a, point_2d &b)
{
    return (a.x == b.x && a.y == b.y);
}

static int are_equal(point_3d &a, point_3d &b)
{
    return (a.x == b.x && a.y == b.y);
}

static void rotate_point(point_2d &point, point_2d &center, float angle)
{
    int x = point.x;
    int y = point.y;

    point.x = center.x + (x - center.x) * cos(angle) + (y - center.y) * sin(angle);
    point.y = center.y + (y - center.y) * cos(angle) - (x - center.x) * sin(angle);
}