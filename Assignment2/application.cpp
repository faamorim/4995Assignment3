#include "application.h"


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: The WindowProc for the application
Parameters:
[in] hWnd - a Handle to the application window
[in] uMessage - the current message to be handled by the Window Proc (or to pass it back up)
[in] wParam, lParam - Params
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
long CALLBACK Application::WndProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam) {

    InputReader* inputreader = (InputReader*)GetWindowLongPtr(hWnd, 0);
    switch (uMessage) {
    case WM_CREATE:
    {
        return 0;
    }
    case WM_PAINT:
    {
        ValidateRect(hWnd, NULL);//basically saying - yeah we took care of any paint msg without any overhead
        return 0;
    }
    case WM_DESTROY:
    {
        PostQuitMessage(0);
        return 0;
    }
    case WM_LBUTTONDOWN:
    {
        inputreader->ReadMouseInput(WM_LBUTTONDOWN, LOWORD(lParam), HIWORD(lParam));
        //// compute the ray in view space given the clicked screen point
        //d3d::Ray ray = CalcPickingRay(LOWORD(lParam), HIWORD(lParam));

        //// transform the ray to world space
        //D3DXMATRIX view;
        //Device->GetTransform(D3DTS_VIEW, &view);

        //D3DXMATRIX viewInverse;
        //D3DXMatrixInverse(&viewInverse, 0, &view);

        //TransformRay(&ray, &viewInverse);

        //// test for a hit
        //if (RaySphereIntTest(&ray, &BSphere))
        //    ::MessageBox(0, "Hit!", "HIT", 0);
        break;
    }
    case WM_KEYDOWN:
    {
        inputreader->ReadKeyboardInput(WM_KEYDOWN, wParam);
        break;
    }
    case WM_KEYUP:
    {
        inputreader->ReadKeyboardInput(WM_KEYUP, wParam);
        break;
    }
    }
    return DefWindowProc(hWnd, uMessage, wParam, lParam);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Constructs an application based on the app name and the handle to the Instace.
Parameters:
[in] strAppName - the name of the applciation to be registered
[in] hInstance - the instance of the handle
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
Application::Application(string strAppName, HINSTANCE hInstance)
        : strAppName(strAppName), hInstance(hInstance){
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = sizeof(InputReader*); //wnd space for info carrying;
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.hbrBackground = (HBRUSH)GetStockObject(DKGRAY_BRUSH);
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hIconSm = LoadIcon(NULL, IDI_HAND);
    wc.hCursor = LoadCursor(NULL, IDC_CROSS);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = strAppName.c_str();

    RegisterClassEx(&wc);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Destructor
No Parameters.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
Application::~Application() {

}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Returns the Application Name
No Parameters.
Returns: The LPCSTR of the Application Name
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
LPCSTR Application::getAppName() {
    return strAppName.c_str();
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Returns the Application Instance Handle
No Parameters.
Returns: The Handle to the Instance of the Application
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
HINSTANCE Application::getHInstance() {
    return hInstance;
}