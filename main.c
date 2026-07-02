#define UNICODE //DONT FORGET TO INCLUDE UNICODE
#define _UNICODE
#include <stdio.h>
#include <windows.h>
#include <stdbool.h>

#define MAINMENU 1
#define TOOLSMENU 2

bool quit = false; //Handles termination of main loop

LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void AddMenus(HWND hWnd); 
void AddToolbar(HWND hWnd);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int nCmdShow)
{
    
    //1. Create Window Class
    WNDCLASS wc = {0};
    const wchar_t CLASS_NAME[] = L"PaintClass";

    //1.1 Initialize mandatory members
    wc.lpszClassName = CLASS_NAME; //Long string 
    wc.lpfnWndProc = WindowProcedure; //Pass address of window procedure and define function
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);

    //2. Register Window Class
    if (!RegisterClass(&wc)) {
        return -1;
    }

    //3. Create Window and store handle
    HWND hWnd = CreateWindow(
        CLASS_NAME,                     //Window Class name
        L"Paint in C",                  //Window name
        WS_OVERLAPPEDWINDOW,            //Window style
        CW_USEDEFAULT, CW_USEDEFAULT,   //X and y coordinate of the upper left corner of the window
        500, 500,                       //Window width and height                   
        NULL,                           //Handle of the parent window (NULL because independent window)
        NULL,                           //Handle to a menu
        hInstance,                      //Handle to the instance of the module to be associated with the window
        NULL                            //LPVOID (idk, not important yet)
    );

    if (hWnd == NULL) {
        return -1;
    }

    //4. Show window
    ShowWindow(hWnd, nCmdShow);

    //5. Create program loop and message loop
    while (!quit) {
        MSG msg = {0};
        while (GetMessage(&msg, NULL, 0, 0)) {
            TranslateMessage(&msg); //Translates virtual-key messages into character messsages
            DispatchMessage(&msg);  //Dispatches message to a window procedure
        }
    }

    return 0;
}

LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    //1.2 Create switch statements for messages
    switch (uMsg) {
        case WM_COMMAND: {
            switch (wParam) {
                case MAINMENU: {
                    //ADD SOMETHING HERE
                }
                case TOOLSMENU: {
                    printf("button pressed\n");
                }
            }
        }

        case WM_CREATE: { //Message flag is passed whenever the window is created
            AddMenus(hWnd);
            AddToolbar(hWnd);
        } break;

        case WM_QUIT:
        case WM_DESTROY: {
            // PostQuitMessage(0); //Indicates to system that thread has made a request to quit (exit code 0)  
            quit = true; //Will terminate the main loop
        } break;

        default: {//Default function to take care of the messages
            return DefWindowProcW(hWnd, uMsg, wParam, lParam);
        } break;
    }
}

void AddMenus(HWND hWnd)
{
    HMENU hMenu_Main = CreateMenu(); //Creates main menu bar

    //Menus inside main menu bar
    AppendMenu(hMenu_Main, MF_STRING, MAINMENU, L"File"); 
    AppendMenu(hMenu_Main, MF_STRING, MAINMENU, L"Edit"); 
    AppendMenu(hMenu_Main, MF_STRING, MAINMENU, L"View"); 

    SetMenu(hWnd, hMenu_Main);
}

void AddToolbar(HWND hWnd)
{   
    //Create another class for toolbar 'div'
    WNDCLASS tc = {0};
    const wchar_t tCLASS_NAME[] = L"Toolbar";

    tc.lpszClassName = tCLASS_NAME;
    tc.lpfnWndProc = DefWindowProc;
    tc.hbrBackground = (HBRUSH)COLOR_GRAYTEXT;
    tc.hInstance = NULL;

    if(!RegisterClass(&tc)) {
        return;
    }

    RECT MainDimensions = {0}; //NEED TO FIX
    GetWindowRect(hWnd, &MainDimensions);
    HWND hToolbar = CreateWindow(
        tCLASS_NAME,
        L"",
        WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN,
        0, 0,
        (int)MainDimensions.right, (int)(MainDimensions.bottom / 10),
        hWnd,
        NULL,
        NULL,
        NULL
    );

    RECT ToolbarDimensions = {0};
    GetWindowRect(hToolbar, &ToolbarDimensions);
    printf("left: %d\n top: %d\n right: %d\n bottom: %d\n",
        ToolbarDimensions.left,
        ToolbarDimensions.top,
        ToolbarDimensions.right,
        ToolbarDimensions.bottom);
    CreateWindow(
        L"Button",
        L"Brush",
        WS_VISIBLE | WS_CHILD,
        (int)ToolbarDimensions.left, (int)ToolbarDimensions.top,
        160, 50,
        hToolbar,
        (HMENU)TOOLSMENU,
        NULL,
        NULL
    );
}