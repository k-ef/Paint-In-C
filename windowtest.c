#include <windows.h>
#include <stdbool.h>

bool quit = false;

struct {
    int width;
    int height;
    uint32_t *pixels;    
} frame = {0}

BITMAPINFO frame_bitmap_info;
HBITMAP frame_bitmap = 0;
HDC frame_device_context = 0;

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
        
        // Do game stuff here
    }
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg) {
        case WM_QUIT:
        case WM_DESTROY: {
            quit = true;
        } break;

        default: {
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
        } break;
    }
    return 0;
}


