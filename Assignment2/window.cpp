#include "window.h"



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Create a new Window for the given Application
Parameters:
[in] application - the Application that handles the window
[in] width - width of the window
[in] height - height of the window
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
Window::Window(Application application, int width, int height, InputReader* inputreader)
        : App(application), Width(width), Height(height), inputreader(inputreader) {
    LPCSTR strAppName = application.getAppName();
    hWnd = CreateWindowEx(NULL,
        strAppName,
        strAppName,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        width, height,
        NULL,
        NULL,
        application.getHInstance(),
        NULL);
    SetWindowLongPtr(hWnd, 0, (LONG)inputreader);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Default destructor
No Parameters.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
Window::~Window()
{
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Show the current window
Parameters:
[in] iCmdShow - the Show parameter for showing the window.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void Window::Show(int iCmdShow) {
    ShowWindow(hWnd, iCmdShow);
    UpdateWindow(hWnd);
}