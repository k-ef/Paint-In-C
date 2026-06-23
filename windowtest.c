#include <stdio.h>
#include <windows.h>
#include <stdbool.h>
#include <stdint.h>

bool quit = false;

struct {
    int width;
    int height;
    uint32_t *pixels;    
} frame = {0};

bool keyboard[256] = {0};
struct { //stores the cursor position
    int x, y;
    uint8_t buttons;
} mouse;
enum {MOUSE_LEFT = 0b1, MOUSE_MIDDLE = 0b10, MOUSE_RIGHT = 0b100, MOUSE_x1 = 0b1000, MOUSE_x2 = 0b10000};

#if RAND_MAX == 32767
#define Rand32() ((rand() << 16) + (rand() << 1) + (rand() & 1))
#else
#define Rand32() rand()
#endif

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
        //x  y    width height
        640, 300, 700, 700,

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

        static int keyboard_x = 0, keyboard_y = 0;
        if (keyboard[VK_RIGHT] || keyboard ['D']) ++keyboard_x;
        if (keyboard[VK_LEFT] || keyboard ['A']) --keyboard_x;
        if (keyboard[VK_UP] || keyboard ['W']) ++keyboard_x;
        if (keyboard[VK_DOWN] || keyboard ['S']) --keyboard_x;

        if (keyboard_x < 0) keyboard_x = 0;
        if (keyboard_x > frame.width - 1) keyboard_x = frame.width - 1;
        if (keyboard_y < 0) keyboard_y = 0;
        if (keyboard_y > frame.height - 1) keyboard_y = frame.width - 1;

        for (int i = 0; i < 1000; i++) frame.pixels[Rand32() % (frame.width * frame.height)] = 0;

        frame.pixels[keyboard_x + keyboard_y * frame.width] = 0x00ffffff;
        if (mouse.buttons & MOUSE_LEFT) frame.pixels [mouse.x + mouse.y * frame.width] = 0x00ffffff; //check if left mouse button is pressed
                                                                                                     //then set pixel on the mouse position to white

        InvalidateRect(hwnd, NULL, FALSE); //marks a section of the window as invalid and needing to be redrawn
        UpdateWindow(hwnd); //updates a wm_paint message to window process message
    }
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static bool has_focus = true;

    switch(uMsg) {
        case WM_QUIT:
        case WM_DESTROY: {
            quit = true;
        } break;

        case WM_KILLFOCUS: {
            has_focus = false;
            memset(keyboard, 0, 256 * sizeof(keyboard[0]));
            mouse.buttons = 0;
        } break;
        
        case WM_SETFOCUS: has_focus = true; break;

        case WM_SYSKEYDOWN:
        case WM_SYSKEYUP:
        case WM_KEYDOWN:
        case WM_KEYUP: {
            if (has_focus) {
                static bool key_is_down, key_was_down;
                key_is_down = ((lParam & (1 << 31)) == 0); //check current and previous state of the key by extracting the
                key_was_down = ((lParam & (1 << 30)) != 0); //31st and 32nd bits of lParam
                if (key_is_down != key_was_down) { //we do 'above' to ignore key repeat events
                    keyboard[(uint8_t)wParam] = key_is_down; //wparam stores key index
                    if (key_is_down) {
                        switch(wParam) {
                            case VK_ESCAPE: quit = true; break;
                        }
                    }
                }
            }
        }

        case WM_MOUSEMOVE: {
            mouse.x = LOWORD(lParam);
            mouse.y = frame.height - 1 - HIWORD(lParam);
        } break;

        case WM_LBUTTONDOWN: mouse.buttons |= MOUSE_LEFT; break; //when pressed, use bitwise OR to set to true
        case WM_LBUTTONUP: mouse.buttons &= ~MOUSE_LEFT; break; //when release, use bitwise AND the inverse to set to false
        case WM_MBUTTONDOWN: mouse.buttons |= MOUSE_MIDDLE; break;
        case WM_MBUTTONUP: mouse.buttons &= ~MOUSE_MIDDLE; break;
        case WM_RBUTTONDOWN: mouse.buttons |= MOUSE_RIGHT; break;
        case WM_RBUTTONUP: mouse.buttons &= ~MOUSE_RIGHT; break;

        // case WM_XBUTTONDOWN: { //extra button (i.e. side buttons on a mouse)
        //                                                                  //DO THIS IF YOU WANT TO
        // }

        case WM_MOUSEWHEEL: {
            printf("%s\n", wParam & 0b10000000000000000000000000000000 ? "Down" : "Up"); //scroll direction is stored in the 32nd bit of the wparam
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
            frame_bitmap = CreateDIBSection(NULL, &frame_bitmap_info, DIB_RGB_COLORS, (void **)&frame.pixels, 0, 0);
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


