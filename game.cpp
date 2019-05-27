#include "game.h"


Game::Game() {

}


Game::~Game()
{
    if (pDevice)
        pDevice->Release();

    if (pD3D)
        pD3D->Release();
}

int Game::countFrameRate() {
    tStart = tEnd;
    QueryPerformanceCounter(&tEnd);
    int dif = (int)(tEnd.QuadPart - tStart.QuadPart);
    if (dif > 0) {
        return (int)(1000000 / dif);
    }
    return 0;
}

boolean Game::Init(Window window, int width, int height, boolean fullScreen) {
    HRESULT r = 0;//return values
    
    pD3D = Direct3DCreate9(D3D_SDK_VERSION);//COM object
    if (pD3D == NULL) {
        SetError("Could not create IDirect3D9 object");
        return false;
    }

    r = InitDirect3DDevice(window.getHWnd(), width, height, !fullScreen, D3DFMT_X8R8G8B8, pD3D, &pDevice);
    if (FAILED(r)) {//FAILED is a macro that returns false if return value is a failure - safer than using value itself
        SetError("Initialization of the device failed");
        return false;
    }

    r = fonts.Init(pDevice, 22);
    if (FAILED(r)) {
        SetError("Initialization of the font failed");
        return false;
    }

    pBackSurf = 0;
    if (!pDevice) {
        SetError("Cannot render because there is no device");
        return E_FAIL;
    }

    LPDIRECT3DSURFACE9 pSurf = 0;

    r = LoadBitmapToSurface("baboon.bmp",
        &pSurf, pDevice);
    if (FAILED(r)) {
        SetError("could not load bitmap surface");
    }

    pSurface = 0;

    r = pDevice->CreateOffscreenPlainSurface(width, height, D3DFMT_X8R8G8B8, D3DPOOL_SYSTEMMEM, &pSurface, NULL);
    if (FAILED(r))
        SetError("could not create surface");

    r = D3DXLoadSurfaceFromSurface(pSurface, NULL, NULL, pSurf, NULL, NULL, D3DX_FILTER_TRIANGLE, 0);
    if (FAILED(r))
        SetError("did not copy surface");
    return true;
}

void Game::Start() {
    MSG msg;
    while (TRUE) {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT)
                break;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else {
            GameLoop();
        }
    }
}

int Game::GameLoop() {
    frameRate = countFrameRate();
    Render();
    if (GetAsyncKeyState(VK_ESCAPE))
        PostQuitMessage(0);

    return S_OK;
}



int Game::Render() {
    HRESULT r;
    pBackSurf = 0;
    if (!pDevice) {
        SetError("Cannot render because there is no device");
        return E_FAIL;
    }

    //clear the display arera with colour black, ignore stencil buffer
    pDevice->Clear(0, 0, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 25), 1.0f, 0);

    //get pointer to backbuffer
    r = pDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackSurf);
    if (FAILED(r)) {
        SetError("Couldn't get backbuffer");
    }

    r = pDevice->UpdateSurface(pSurface, NULL, pBackSurf, NULL);
    if (FAILED(r))
        SetError("did not copy surface");

    pBackSurf->Release();//release lock

    pDevice->BeginScene();
    string fps = to_string(frameRate);
    string displayString = fps + "fps";
    fonts.Print(displayString);
    pDevice->EndScene();
    pDevice->Present(NULL, NULL, NULL, NULL);//swap over buffer to primary surface
    return S_OK;
}

int Game::LoadBitmapToSurface(const char* PathName, LPDIRECT3DSURFACE9* ppSurface, LPDIRECT3DDEVICE9 pDevice) {
    HRESULT r;
    HBITMAP hBitmap;
    BITMAP Bitmap;

    hBitmap = (HBITMAP)LoadImage(NULL, PathName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    if (hBitmap == NULL) {
        SetError("Unable to load bitmap");
        return E_FAIL;
    }

    GetObject(hBitmap, sizeof(BITMAP), &Bitmap);
    DeleteObject(hBitmap);//we only needed it for the header info to create a D3D surface

    //create surface for bitmap
    r = pDevice->CreateOffscreenPlainSurface(Bitmap.bmWidth, Bitmap.bmHeight, D3DFMT_X8R8G8B8, D3DPOOL_SCRATCH, ppSurface, NULL);

    if (FAILED(r)) {
        SetError("Unable to create surface for bitmap load");
        return E_FAIL;
    }
    //load bitmap onto surface
    r = D3DXLoadSurfaceFromFile(*ppSurface, NULL, NULL, PathName, NULL, D3DX_DEFAULT, 0, NULL);
    if (FAILED(r)) {
        SetError("Unable to laod file to surface");
        return E_FAIL;
    }

    return S_OK;
}




int InitDirect3DDevice(HWND hWndTarget, int Width, int Height, BOOL bWindowed, D3DFORMAT FullScreenFormat, LPDIRECT3D9 pD3D, LPDIRECT3DDEVICE9* ppDevice) {
    D3DPRESENT_PARAMETERS d3dpp;//rendering info
    D3DDISPLAYMODE d3ddm;//current display mode info
    HRESULT r = 0;

    if (*ppDevice)
        (*ppDevice)->Release();

    ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));
    r = pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm);
    if (FAILED(r)) {
        SetError("Could not get display adapter information");
        return E_FAIL;
    }

    d3dpp.BackBufferWidth = Width;
    d3dpp.BackBufferHeight = Height;
    d3dpp.BackBufferFormat = bWindowed ? d3ddm.Format : FullScreenFormat;
    d3dpp.BackBufferCount = 1;
    d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;//D3DSWAPEFFECT_COPY;
    d3dpp.hDeviceWindow = hWndTarget;
    d3dpp.Windowed = bWindowed;
    d3dpp.EnableAutoDepthStencil = TRUE;
    d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
    d3dpp.FullScreen_RefreshRateInHz = 0;//default refresh rate
    d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
    d3dpp.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;

    r = pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWndTarget, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, ppDevice);
    if (FAILED(r)) {
        SetError("Could not create the render device");
        return E_FAIL;
    }

    //	g_DeviceHeight = Height;
    //	g_DeviceWidth = Width;

    //	g_SavedPresParams = d3dpp;
    return S_OK;
}

void SetError(const char* szFormat, ...) {
    char szBuffer[1024];
    va_list pArgList;

    va_start(pArgList, szFormat);

    _vsntprintf_s(szBuffer, sizeof(szBuffer) / sizeof(char), szFormat, pArgList);

    va_end(pArgList);

    OutputDebugString(szBuffer);
    OutputDebugString("\n");
    printf("error!!");
}

