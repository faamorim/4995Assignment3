#pragma once

#include <Windows.h>
#include <string>
#include "InputReader.h"

using namespace std;

class Application
{
    string strAppName;
    static long CALLBACK WndProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam);
    WNDCLASSEX wc;
    HINSTANCE hInstance;
public:
    Application(string strAppName, HINSTANCE hInstance);
    ~Application();
    LPCSTR getAppName();
    HINSTANCE getHInstance();
};
