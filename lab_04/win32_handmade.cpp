z#include <windows.h>
#include <stdint.h>
#include <synchapi.h>
#include <xinput.h>
#include <dsound.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "renderer.hpp"

struct 
bitmap_buff {
    BITMAPINFO Info;
    LPVOID Memory;
    LONG Width;
    LONG Height;
};

struct
window_dimension {
    LONG Width;
    LONG Height;
};

static bool GlobalRunning;
static bool GlobalRotating;
static bitmap_buff GlobalBitmap;

#define XINPUT_GET_STATE(name) DWORD WINAPI name(DWORD dwUserIndex, XINPUT_STATE *pState)
typedef XINPUT_GET_STATE(xinput_get_state);
XINPUT_GET_STATE(XInputGetStateStub) {
    return ERROR_DEVICE_NOT_CONNECTED;
}
static xinput_get_state *XInputGetState_ = XInputGetStateStub;
#define XInputGetState XInputGetState_

#define XINPUT_SET_STATE(name) DWORD WINAPI name(DWORD dwUserIndex, XINPUT_VIBRATION *pVibration)
typedef XINPUT_SET_STATE(xinput_set_state);
XINPUT_SET_STATE(XInputSetStateStub) {
    return ERROR_DEVICE_NOT_CONNECTED;
}
static xinput_set_state *XInputSetState_ = XInputSetStateStub;
#define XInputSetState XInputSetState_

static void Win32LoadXInput(void);
static window_dimension GetWindowDimention(HWND Window);
LRESULT Wndproc(HWND Window, UINT Message, WPARAM wParam, LPARAM lParam);
static void Win32ResizeDIBSection(bitmap_buff *GlobalBitmap, LONG Width, LONG Height);
static void Win32UpdateWindow(HDC DeviceContext, LONG WindowWidth, LONG WindowHeight, 
                                bitmap_buff *GlobalBitmap, LONG X, LONG Y, LONG Width, LONG Height);

polygon_2d *GeneratePolygons2DData(int nMaxPolygons, int nMaxVertexes, int xMin, int xMax, int yMin, int yMax);
polygon_3d *GeneratePolygons3DData(int nPolygons, int nMaxVertexes, int xMin, int xMax, int yMin, int yMax);

bool FReadPolygons3DData(polygons_3d &p, char *fileName);

#define N_THREADS 4

void
RenderAll(bitmap_buff *GlobalBitmap, polygons_3d &Polygons3D) {
    FillBuffer((uint32_t *)GlobalBitmap->Memory, GlobalBitmap->Width, GlobalBitmap->Height, 0xFFFFFF);

    #define N_COLORS 10
    uint32_t colors[10] = {
        0xFFFF0000, // Bright Red
        0xFF00FF00, // Bright Green
        0xFF0000FF, // Bright Blue
        0xFFFFFF00, // Bright Yellow
        0xFFFF00FF, // Bright Magenta
        0xFF00FFFF, // Bright Cyan
        0xFFFFA500, // Bright Orange
        0xFF800080, // Bright Purple
        0xFF008000, // Bright Dark Green
        0xFF808080  // Bright Gray
    };

    //ZBuffer((uint32_t *)GlobalBitmap->Memory, GlobalBitmap->Width, GlobalBitmap->Height, Polygons3D);
    ZBufferM((uint32_t *)GlobalBitmap->Memory, GlobalBitmap->Width, GlobalBitmap->Height, Polygons3D, N_THREADS);
    //ZBufferM((uint32_t *)GlobalBitmap->Memory, GlobalBitmap->Width, GlobalBitmap->Height, Polygons3D, GlobalBitmap->Height);

    point_3d center = {GlobalBitmap->Width / 2, GlobalBitmap->Height / 2, 0};

    if (GlobalRotating)
        RotatePolygons3D(Polygons3D, center, 1, 1, 1);
}

polygon_2d GeneratePolygon2DData(int nMaxVertexes, int xMin, int xMax, int yMin, int yMax)
{
    polygon_2d polygon = {};
    polygon.nVertexes = 3 + (rand() % (nMaxVertexes - 3));
    polygon.size = sizeof(point_2d) * polygon.nVertexes;
    polygon.v = (point_2d *)malloc(sizeof(point_2d) * polygon.nVertexes);

    int Dx = xMax - xMin;
    int Dy = yMax - yMin;

    for (int i = 0; i < polygon.nVertexes; ++i) {
        polygon.v[i].x = xMin + rand() % Dx;
        polygon.v[i].y = yMin + rand() % Dy;
    }

    return polygon;
}

polygon_3d FReadPolygon3DData(FILE *f)
{
    polygon_3d p = {};
    assert(fscanf(f, "%zu", &p.nVertexes) >= 0 && p.nVertexes > 0);

    p.v = (point_3d *)malloc(sizeof(point_3d) * p.nVertexes);
    for (int i = 0; i < p.nVertexes; ++i) {
        assert(fscanf(f, "%d %d %d", &p.v[i].x, &p.v[i].y, &p.v[i].z) >= 0);
    }
    
    point_3d e1 = {p.v[0].x - p.v[1].x, p.v[0].y - p.v[1].y, p.v[0].z - p.v[1].z};
    point_3d e2 = {p.v[2].x - p.v[1].x, p.v[2].y - p.v[1].y, p.v[2].z - p.v[1].z};

    p.normal = {e1.y * e2.z - e1.z * e2.y, e1.z * e2.x - e1.x * e2.z, e1.x * e2.y - e1.y * e2.x};

    uint32_t colors[10] = {
        0xFFFF0000, // Bright Red
        0xFF00FF00, // Bright Green
        0xFF0000FF, // Bright Blue
        0xFFFFFF00, // Bright Yellow
        0xFFFF00FF, // Bright Magenta
        0xFF00FFFF, // Bright Cyan
        0xFFFFA500, // Bright Orange
        0xFF800080, // Bright Purple
        0xFF008000, // Bright Dark Green
        0xFF808080  // Bright Gray
    };

    static int i = 0;
    p.color = colors[i % 10];
    ++i;

    return p;
}

bool FReadPolygons3DData(polygons_3d &p, char *fileName)
{
    FILE *f = fopen(fileName, "r");
    if (!f)
        return false;
    
    if (fscanf(f, "%zu", &p.n) < 0 && p.n < 0)
        return false;

    p.p = (polygon_3d *)malloc(sizeof(polygon_3d) * p.n);

    for (int i = 0; i < p.n; ++i) {
        p.p[i] = FReadPolygon3DData(f);
    }

    fclose(f);
    return true;
}

polygon_2d *GeneratePolygons2DData(int nPolygons, int nMaxVertexes, int xMin, int xMax, int yMin, int yMax)
{
    polygon_2d *Polygons = (polygon_2d *)malloc(sizeof(polygon_2d) * nPolygons);

    for (int i = 0; i < nPolygons; ++i) {
        Polygons[i] = GeneratePolygon2DData(nMaxVertexes, xMin, xMax, yMin, yMax);
    }

    return Polygons;
}

polygon_3d GeneratePolygon3DData(int nMaxVertexes, int xMin, int xMax, int yMin, int yMax)
{
    polygon_3d polygon = {};
    polygon.nVertexes = 3 + (rand() % (nMaxVertexes - 3));
    polygon.size = sizeof(point_3d) * polygon.nVertexes;
    polygon.v = (point_3d *)malloc(sizeof(point_3d) * polygon.nVertexes);

    int Dx = xMax - xMin;
    int Dy = yMax - yMin;

    int z = rand() % 3;

    for (int i = 0; i < polygon.nVertexes; ++i) {
        polygon.v[i].x = xMin + rand() % Dx;
        polygon.v[i].y = yMin + rand() % Dy;
        polygon.v[i].z = z;
    }

    return polygon;
}

polygon_3d *GeneratePolygons3DData(int nPolygons, int nMaxVertexes, int xMin, int xMax, int yMin, int yMax)
{
    polygon_3d *Polygons = (polygon_3d *)malloc(sizeof(polygon_3d) * nPolygons);

    for (int i = 0; i < nPolygons; ++i) {
        Polygons[i] = GeneratePolygon3DData(nMaxVertexes, xMin, xMax, yMin, yMax);
    }

    return Polygons;
}

int CALLBACK WinMain(HINSTANCE Instance, HINSTANCE PrevInstance,
                     LPSTR CommandLine, int ShowCode) {  
    srand(time(NULL)); 
    LARGE_INTEGER PerfCounterFreqRes;
    QueryPerformanceFrequency(&PerfCounterFreqRes);
    int64_t PerfCounterFreq = PerfCounterFreqRes.QuadPart;

    WNDCLASS WindowClass = {};

    Win32LoadXInput();

    WindowClass.style = CS_OWNDC|CS_VREDRAW|CS_HREDRAW;
    WindowClass.lpfnWndProc = Wndproc;
    WindowClass.hInstance = Instance;
    WindowClass.lpszClassName = "WindowCGWClass";

    if (RegisterClassA(&WindowClass)) {
        HWND Window = CreateWindowEx(0, WindowClass.lpszClassName, "Course work", WS_OVERLAPPED|WS_MINIMIZEBOX|WS_SYSMENU|WS_VISIBLE|WS_MAXIMIZE,
                  CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 
                  0, 0, Instance, 0);

        if (Window) {
            window_dimension Dimension = GetWindowDimention(Window);
            Win32ResizeDIBSection(&GlobalBitmap, Dimension.Width, Dimension.Height);

            MSG Message;
            HDC DeviceContext = GetDC(Window);

            LARGE_INTEGER LastCounter;
            QueryPerformanceCounter(&LastCounter);

            int64_t LastCyclesCount = __rdtsc();

            polygons_3d Polygons3D;
            FReadPolygons3DData(Polygons3D, "polygons.txt");

            GlobalRunning = true;
            GlobalRotating = false;
            while (GlobalRunning) {
                while (PeekMessageA(&Message, 0, 0, 0, PM_REMOVE)) {
                    if (Message.message == WM_QUIT)
                        GlobalRunning = false;

                    TranslateMessage(&Message);
                    DispatchMessageA(&Message);
                }

                // Rendering test
                RenderAll(&GlobalBitmap, Polygons3D);
                window_dimension Dimension = GetWindowDimention(Window);
                Win32UpdateWindow(DeviceContext, Dimension.Width, Dimension.Height, &GlobalBitmap, 
                                  0, 0, Dimension.Width, Dimension.Height);
                
                LARGE_INTEGER CurCounter;
                QueryPerformanceCounter(&CurCounter);

                int64_t CurCyclesCount = __rdtsc();

                int64_t FrameCounts = CurCounter.QuadPart - LastCounter.QuadPart;
                int32_t MSPerFrame = (int32_t)(1000 * FrameCounts) / PerfCounterFreq;
                int32_t FPS = (int32_t)PerfCounterFreq / FrameCounts;
                int32_t MCPerFrame = (int32_t)((CurCyclesCount - LastCyclesCount) / (1000 * 1000));

                char Buffer[256];
                wsprintf(Buffer, "%dms / f,  %df / s,  %dmc / f\n", MSPerFrame, FPS, MCPerFrame);
                OutputDebugStringA(Buffer);

                LastCounter = CurCounter;
                LastCyclesCount = CurCyclesCount;
            }
            ReleaseDC(Window, DeviceContext);
        }
        else
        {
            // TODO(Talkasi): Loggining
        }
    }
    else 
    {
        // TODO(Talkasi): loggining
    }


    return 0;
}

static void 
Win32LoadXInput(void) {
    HMODULE XInputLib = LoadLibraryA("xinput1_4.dll");
    if (!XInputLib)
        XInputLib = LoadLibraryA("xinput1_3.dll");

    if (!XInputLib)
        XInputLib = LoadLibraryA("xinput9_1_0.dll");
    
    if (XInputLib) {
        XInputGetState = (xinput_get_state *)GetProcAddress(XInputLib, "XInputGetState");
        XInputSetState = (xinput_set_state *)GetProcAddress(XInputLib, "XInputSetState");
    }
}

static window_dimension
GetWindowDimention(HWND Window) {
    window_dimension Result;

    RECT ClientRect;
    GetClientRect(Window, &ClientRect);
    Result.Width = ClientRect.right - ClientRect.left;
    Result.Height = ClientRect.bottom - ClientRect.top;

    return Result;
}

static void
Win32ResizeDIBSection(bitmap_buff *GlobalBitmap, LONG Width, LONG Height) {
    if (GlobalBitmap->Memory)
        VirtualFree(GlobalBitmap->Memory, 0, MEM_RELEASE);

    GlobalBitmap->Width = Width;
    GlobalBitmap->Height = Height;

    GlobalBitmap->Info.bmiHeader.biSize = sizeof(GlobalBitmap->Info.bmiHeader);
    GlobalBitmap->Info.bmiHeader.biWidth = GlobalBitmap->Width;
    GlobalBitmap->Info.bmiHeader.biHeight = -GlobalBitmap->Height;
    GlobalBitmap->Info.bmiHeader.biPlanes = 1;
    GlobalBitmap->Info.bmiHeader.biBitCount = 32;
    GlobalBitmap->Info.bmiHeader.biCompression = BI_RGB;

    int BytesPerPixel = 4;
    SIZE_T BitmapMemorySize = BytesPerPixel * GlobalBitmap->Width * GlobalBitmap->Height;
    GlobalBitmap->Memory = VirtualAlloc(0, BitmapMemorySize, MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);
}

static void
Win32UpdateWindow(HDC DeviceContext, LONG WindowWidth, LONG WindowHeight, 
                  bitmap_buff *GlobalBitmap, LONG X, LONG Y, LONG Width, LONG Height) {
    // StretchDIBits(DeviceContext, 
    //               0, 0, WindowWidth, WindowHeight,
    //               0, 0, GlobalBitmap->Width, GlobalBitmap->Height,
    //               GlobalBitmap->Memory, &GlobalBitmap->Info, 
    //               DIB_RGB_COLORS, SRCCOPY);
    StretchDIBits(DeviceContext,
                0, 0, WindowWidth, WindowHeight,
                0, 0, WindowWidth, WindowHeight,
                GlobalBitmap->Memory, &GlobalBitmap->Info, 
                DIB_RGB_COLORS, 
                SRCCOPY);
}

LRESULT Wndproc(HWND Window, UINT Message, WPARAM wParam, LPARAM lParam) {
    LRESULT Result = 0;
    switch(Message) {
    case WM_CLOSE:
        {
            GlobalRunning = false;
        } break;

    case WM_DESTROY:
        {
            GlobalRunning = false;
        } break;

    case WM_SYSKEYDOWN:
    case WM_SYSKEYUP:
    case WM_KEYDOWN:
    case WM_KEYUP:
        {
            uint32_t VKCode = wParam;
            bool WasDown = ((lParam & (1 << 30)) != 0);
            bool IsDown = ((lParam & (1 << 31)) == 0);

            switch (VKCode) {
            case VK_LEFT:
            case 'A':
                {
                    // Yo += 10;
                }

            case VK_DOWN:
            case 'S':
                {
                    // Xo -= 10;
                }

            case VK_RIGHT:
            case 'D':
                {
                    // Yo -= 10;
                }

            case VK_UP:
            case 'W':
                {
                    // Xo += 10;
                }

            case VK_ESCAPE:
                {

                }

            case VK_SPACE:
                {
                    if (Message == WM_KEYDOWN && !WasDown)
                        GlobalRotating = !GlobalRotating;
                    break;
                }

            case VK_F4:
                {
                    bool IsAltKeyPressed = ((lParam & (1 << 29)) != 0);
                    if (IsAltKeyPressed)
                        GlobalRunning = false;
                }
            }
        } break;


    case WM_SIZE:
        {
        } break;

    case WM_ACTIVATEAPP:
        {
            OutputDebugStringA("WM_ACTIVATEAPP\n");
        } break;

    case WM_PAINT:
        {
            PAINTSTRUCT Paint;
            HDC DeviceContext = BeginPaint(Window, &Paint);
            LONG X = Paint.rcPaint.left;
            LONG Y = Paint.rcPaint.top;
            int Width = Paint.rcPaint.right - Paint.rcPaint.left;
            int Height = Paint.rcPaint.bottom - Paint.rcPaint.top;
            
            window_dimension Dimension = GetWindowDimention(Window);
            Win32UpdateWindow(DeviceContext, Dimension.Width, Dimension.Height,
                              &GlobalBitmap, X, Y, Dimension.Width, Dimension.Height);
            EndPaint(Window, &Paint);
        } break;

    default:
        {
            Result = DefWindowProcA(Window, Message, wParam, lParam);
        }
    }

    return Result;
}
