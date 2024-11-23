#ifndef RENDERER_HPP
#define RENDERER_HPP
#include <windows.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

#define Pi32 3.14159265359f

struct point_2d {
    int x;
    int y;
};

struct point_3d {
    int x;
    int y;
    int z;
};

struct rect_2d {
    point_2d minP;
    point_2d maxP;
};

struct polygon_2d {
    point_2d *v;
    size_t nVertexes;
    size_t size;
    uint32_t color;
};

struct polygon_3d {
    point_3d *v;
    size_t nVertexes;
    size_t size;
    uint32_t color;
    point_3d normal;
};

struct polygons_3d {
    polygon_3d *p;
    size_t n;
    size_t size;
    uint32_t color;
};

void FillBuffer(uint32_t *BitmapBuffer, int BitmapWidth, int BitmapHeight, uint32_t Color);
void ZBuffer(uint32_t *BitmapBuffer, int BitmapWidth, int BitmapHeight, polygons_3d &p);
void ZBufferMultitherad(uint32_t *BitmapBuffer, int BitmapWidth, int BitmapHeight, polygons_3d &p);
void ZBufferM(uint32_t *BitmapBuffer, int BitmapWidth, int BitmapHeight, polygons_3d &Polygons, int nThreads);
void ZBufferM1(uint32_t *BitmapBuffer, int BitmapWidth, int BitmapHeight, polygons_3d &Polygons, int nThreads);
void ZBufferM2(uint32_t *BitmapBuffer, int BitmapWidth, int BitmapHeight, polygons_3d &Polygons, int nThreads);

void DrawPoint2D(uint32_t *BitmapBuffer, int BitmapWidth, point_2d P, uint32_t Color);
void DrawLine2D(uint32_t *BitmapBuffer, int BitmapWidth, point_2d P1, point_2d P2, uint32_t Color);
void DrawTriangle2D(uint32_t *BitmapBuffer, int BitmapWidth, point_2d P1, point_2d P2, point_2d P3, uint32_t Color);
void FillTriangle2D(uint32_t *BitmapBuffer, int BitmapWidth, point_2d P1, point_2d P2, point_2d P3, uint32_t Color);
void FillPolygon2D(uint32_t *BitmapBuffer, int BitmapWidth, point_2d *Points, int n, uint32_t Color);

void RotatePoint3D(point_3d &point, point_3d &rotate_center, double yaw, double pitch, double roll);
void RotatePolygons3D(polygons_3d &polygons, point_3d &rotate_center, double yaw, double pitch, double roll);


DWORD WINAPI ProcessPolygons(LPVOID arg);
void CombineBuffers(uint32_t *BitmapBuffer, uint32_t *tempBuffer, int BitmapWidth, int BitmapHeight);

#endif // RENDERER_HPP
