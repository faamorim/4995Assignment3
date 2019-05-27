#pragma once

#include <windows.h>
#include "application.h"
#include "InputReader.h"

class Window
{
    InputReader* inputreader;
    HWND hWnd;
    int Width;
    int Height;
    Application App;
public:

    Window(Application application, int width, int height, InputReader* inputreader);
    ~Window();
    void Show(int iCmdShow);
    HWND GetHWnd() { return hWnd; };
    int GetWidth() { return Width; };
    int GetHeight() { return Height; };
    Application GetApplication() { return App; };
};


