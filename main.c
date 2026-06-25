#include <stdio.h>
#include <windows.h>

LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int nCmdShow)
{
    //1. Create Window Class
    WNDCLASSEXA wc = {0};

    //1.1 Initialize mandatory members
    wc.cbSize = sizeof(WNDCLASSEXA); 
    wc.lpfnWndProc = WindowProcedure; //Pass address of window procedure and define function
    wc.lpszClassName = (LPCTSTR) L"PaintClass"; //Long string 

    //2. Register Window Class
    if (!RegisterClassEx(&wc)) {
        return -1;
    }

    //3. Create Window
    CreateWindowW(
        L"PaintClass",          //Window Class name
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

    //4. Create a message loop
    MSG msg = {0};
    while (GetMessage(&msg, NULL, (UINT) NULL, (UINT) NULL)) {
        TranslateMessage(&msg); //Translates virtual-key messages into character messsages
        DispatchMessage(&msg);  //Dispatches message to a window procedure
    }
    return 0;
}

LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    //1.2 Create switch statements for messages
    switch (uMsg) {
        case WM_DESTROY:
            PostQuitMessage(0); //Indicates to system that thread has made a request to quit (exit code 0)  
            break;
        default: //Default function to take care of the messages
            return DefWindowProcW(hWnd, uMsg, wParam, lParam);
    }
}