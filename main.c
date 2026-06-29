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
    HWND hwnd = CreateWindow(
        CLASS_NAME,          //Window Class name
        L"Paint in C",          //Window name
        WS_OVERLAPPEDWINDOW,    //Window style
        CW_USEDEFAULT,          //X-coordinate of the upper left corner of the window
        CW_USEDEFAULT,          //Y-coordinate of the upper left corner of the window
        500,                    //Window width
        500,                    //Window height
        NULL,                   //Handle of the parent window (NULL because independent window)
        NULL,                   //Handle to a menu
        hInstance,              //Handle to the instance of the module to be associated with the window
        NULL                   //LPVOID (idk, not important yet)
    );

    if (hwnd == NULL) {
        return -1;
    }

    //4. Show window
    ShowWindow(hwnd, nCmdShow);

    //5. Create program loop and message loop
    while (!quit) {
        MSG msg = {0};
        while (GetMessage(&msg, NULL, 0, 0)) {
            TranslateMessage(&msg); //Translates virtual-key messages into character messsages
            DispatchMessage(&msg);  //Dispatches message to a window procedure
        }
        //MAIN LOOP
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
                    //ADD SOMETHING HERE
                }
            }
        }

        case WM_CREATE: { //Message flag is passed whenever the window is created
            AddMenus(hWnd);
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
    HMENU hMenu_Tools = CreateMenu();

    //Submenus inside Paint menu tab
    AppendMenu(hMenu_Tools, MF_STRING, TOOLSMENU, L"Brush");
    AppendMenu(hMenu_Tools, MF_STRING, TOOLSMENU, L"Eraser");
    AppendMenu(hMenu_Tools, MF_STRING, TOOLSMENU, L"Rotate");

    //Menus inside main menu bar
    AppendMenu(hMenu_Main, MF_STRING, MAINMENU, L"File"); 
    AppendMenu(hMenu_Main, MF_STRING, MAINMENU, L"Edit"); 
    AppendMenu(hMenu_Main, MF_STRING, MAINMENU, L"View"); 
    AppendMenu(hMenu_Main, MF_POPUP, (UINT_PTR)hMenu_Tools, L"Paint");

    SetMenu(hWnd, hMenu_Main);
}