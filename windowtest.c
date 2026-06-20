#include <windows.h>
#include <stdbool.h>
#include <stdint.h>

bool quit = false;

struct {
    int width;
    int height;
    uint32_t *pixels;    
} frame = {0};

BITMAPINFO frame_bitmap_info; //bitmap info (metadata/header)
HBITMAP frame_bitmap = 0; //handle to reference a bitmap object in memory
HDC frame_device_context = 0; //device context

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int nCmdShow)
{        
    WNDCLASS wc = {0};
    const wchar_t CLASS_NAME[] = L"Sample Window Class";
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = (LPCSTR)CLASS_NAME;

    RegisterClass(&wc);

    frame_bitmap_info.bmiHeader.biSize = sizeof(frame_bitmap_info.bmiHeader);
    frame_bitmap_info.bmiHeader.biPlanes = 1; //set to 1 always
    frame_bitmap_info.bmiHeader.biBitCount = 32; //bitdepth, bits per pixel
    frame_bitmap_info.bmiHeader.biCompression = BI_RGB;
    frame_device_context = CreateCompatibleDC(0);

    HWND hwnd = CreateWindowEx(
        0,                      //Optional window style
        (LPCSTR)CLASS_NAME,             //Window class
        (LPCSTR)"Paint",               //Window text
        WS_OVERLAPPEDWINDOW,    //Window style

        //Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (hwnd == NULL) {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);

    while(!quit) {
        MSG message;
        while(PeekMessage(&message, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&message);
            DispatchMessage(&message);
        }

        unsigned int p = 0;
        frame.pixels[(p++)%(frame.width*frame.height)] = Rand32();
        frame.pixels[Rand32()%(frame.width*frame.height)] = 0;

        InvalidateRect(hwnd, NULL, FALSE); //marks a section of the window as invalid and needing to be redrawn
        UpdateWindow(hwnd); //updates a wm_paint message to window process message
    }
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg) {
        case WM_QUIT:
        case WM_DESTROY: {
            quit = true;
        } break;

        case WM_PAINT: { //all window drawing (as in how the window is painted not the canvas)
            PAINTSTRUCT paint;
            HDC device_context;
            device_context = BeginPaint(hwnd, &paint);
            BitBlt(device_context,
                   paint.rcPaint.left, paint.rcPaint.top,
                   paint.rcPaint.right - paint.rcPaint.left, paint.rcPaint.bottom - paint.rcPaint.top,
                   frame_device_context, //device context of pixel array
                   paint.rcPaint.left, paint.rcPaint.top, //top left corner to start copying from
                   SRCCOPY); //raster operation code (copy array pixels to window pixels)
            EndPaint(hwnd, &paint);
        } break;

        case WM_SIZE: { 
            frame_bitmap_info.bmiHeader.biWidth = LOWORD(lParam);
            frame_bitmap_info.bmiHeader.biHeight = HIWORD(lParam);

            if (frame_bitmap) {
                DeleteObject(frame_bitmap);
            }
            frame_bitmap = CreateDIBSection(NULL, &frame_bitmap_info, DIB_RGB_COLORS, &frame.pixels, 0, 0);
            SelectObject(frame_device_context, frame_bitmap);

            frame.width = LOWORD(lParam);
            frame.height = HIWORD(lParam);
        }

        default: {
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
        } break;
    }
    return 0;
}


