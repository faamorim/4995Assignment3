#include "d3dUtility.h"

const float d3d::FLT_INFINITY = FLT_MAX;
const float d3d::EPSILON = 0.001f;


const D3DXCOLOR      d3d::WHITE{ D3DCOLOR_XRGB(255, 255, 255) };
const D3DXCOLOR      d3d::BLACK{ D3DCOLOR_XRGB(0,   0,   0) };
const D3DXCOLOR        d3d::RED{ D3DCOLOR_XRGB(255,   0,   0) };
const D3DXCOLOR      d3d::GREEN{ D3DCOLOR_XRGB(0, 255,   0) };
const D3DXCOLOR       d3d::BLUE{ D3DCOLOR_XRGB(0,   0, 255) };
const D3DXCOLOR     d3d::YELLOW{ D3DCOLOR_XRGB(255, 255,   0) };
const D3DXCOLOR       d3d::CYAN{ D3DCOLOR_XRGB(0, 255, 255) };
const D3DXCOLOR    d3d::MAGENTA{ D3DCOLOR_XRGB(255,   0, 255) };

const D3DMATERIAL9 d3d::WHITE_MTRL = InitMtrl(WHITE, WHITE, WHITE, BLACK, 2.0f);
const D3DMATERIAL9 d3d::RED_MTRL = InitMtrl(RED, RED, RED, BLACK, 2.0f);
const D3DMATERIAL9 d3d::GREEN_MTRL = InitMtrl(GREEN, GREEN, GREEN, BLACK, 2.0f);
const D3DMATERIAL9 d3d::BLUE_MTRL = InitMtrl(BLUE, BLUE, BLUE, BLACK, 2.0f);
const D3DMATERIAL9 d3d::YELLOW_MTRL = InitMtrl(YELLOW, YELLOW, YELLOW, BLACK, 2.0f);







/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Initializes the DirectX Object, the DirectX Device, and the DirectX Bitmap Surface
Parameters:
[in] window - the instance of the window
[in] fullScreen - boolean that tells if the application is in full screen
Returns succes or fail
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
boolean d3d::Init(Window window, boolean fullScreen) {
    return Init(window.GetHWnd(), window.GetWidth(), window.GetHeight(), fullScreen);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Initializes the DirectX Object, the DirectX Device, and the DirectX Bitmap Surface
Parameters:
[in] hWnd - handle to the window
[in] width - width of the window
[in] height - height of the window
[in] fullScreen - boolean that tells if the application is in full screen
Returns succes or fail
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
boolean d3d::Init(HWND hWnd, int width, int height, boolean fullScreen) {
    HRESULT r = 0;//return values

    pD3D = Direct3DCreate9(D3D_SDK_VERSION);//COM object
    if (pD3D == NULL) {
        SetError("Could not create IDirect3D9 object");
        return false;
    }

    r = InitDirect3DDevice(hWnd, width, height, !fullScreen, D3DFMT_X8R8G8B8, pD3D, &pDevice);
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

    r = LoadBitmapToSurface("Resources/baboon.bmp",
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


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Initializes the 3D World by setting up DirectX Device Render State and adding Lights
No Parameters.
Returns succes or fail
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
boolean d3d::Init3DWorld() {
    light = Light{ pDevice };


    // Turn on the zbuffer
    if (!pDevice->SetRenderState(D3DRS_ZENABLE, TRUE)) {
        return false;
    }
    return true;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Loads a Bitmap file to a surface
Parameters:
[in] FileName - the name of the file to load the Bitmap from
[in] ppSurface - pointer to where the pointer to the surface will be stored
[in] pDevice - the DirectX Device
Returns succes or fail
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int d3d::LoadBitmapToSurface(const char* FileName, LPDIRECT3DSURFACE9* ppSurface, LPDIRECT3DDEVICE9 pDevice) {
    HRESULT r;
    HBITMAP hBitmap;
    BITMAP Bitmap;

    LPCSTR makeint = MAKEINTRESOURCE(IDB_BABOON);
    HRSRC find = FindResource(NULL, "baboon.bmp", makeint);
    HGLOBAL load = LoadResource(NULL, find);
    hBitmap = (HBITMAP)load;
    hBitmap = (HBITMAP)LoadImage(NULL, FileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

    //hBitmap = (HBITMAP)FindResource(NULL, FileName, "Bitmap");
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
    r = D3DXLoadSurfaceFromFile(*ppSurface, NULL, NULL, FileName, NULL, D3DX_DEFAULT, 0, NULL);
    if (FAILED(r)) {
        SetError("Unable to laod file to surface");
        return E_FAIL;
    }

    return S_OK;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Initializes the DirectX Device
Parameters:
[in] hWndTarget - Handle to the window
[in] Width - Width of the Application
[in] Height - Height of the Application
[in] bWindowed - boolean that is true if the Application is in windowed mode
[in] FullScreenFormat - DirectX format for the full screen Application
[in] pD3D - DirectX Object
[in] ppDevice - Pointer to where the pointer to the DirectX Device will be stored;
Returns: success or fail
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
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
    d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
    d3dpp.FullScreen_RefreshRateInHz = 0;//default refresh rate
    d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
    d3dpp.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;

    r = pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWndTarget, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, ppDevice);
    if (FAILED(r)) {
        SetError("Could not create the render device");
        return E_FAIL;
    }

    //	g_DeviceHeight = Height;
    //	g_DeviceWidth = Width;

    //	g_SavedPresParams = d3dpp;
    return S_OK;
}



D3DMATERIAL9 d3d::InitMtrl(D3DXCOLOR a, D3DXCOLOR d, D3DXCOLOR s, D3DXCOLOR e, float p)
{
    D3DMATERIAL9 mtrl;
    mtrl.Ambient = a;
    mtrl.Diffuse = d;
    mtrl.Specular = s;
    mtrl.Emissive = e;
    mtrl.Power = p;
    return mtrl;
}









d3d::BoundingBox::BoundingBox()
{
     //infinite small 
    _min.x = d3d::FLT_INFINITY;
    _min.y = d3d::FLT_INFINITY;
    _min.z = d3d::FLT_INFINITY;

    _max.x = -d3d::FLT_INFINITY;
    _max.y = -d3d::FLT_INFINITY;
    _max.z = -d3d::FLT_INFINITY;
}

bool d3d::BoundingBox::isPointInside(D3DXVECTOR3& p)
{
    if (p.x >= _min.x && p.y >= _min.y && p.z >= _min.z &&
        p.x <= _max.x && p.y <= _max.y && p.z <= _max.z)
    {
        return true;
    }
    else
    {
        return false;
    }
}

d3d::BoundingSphere::BoundingSphere()
{
    _radius = 0.0f;
}

//
// Functions
//
d3d::Ray d3d::CalcPickingRay(int x, int y)
{
    float px = 0.0f;
    float py = 0.0f;

    D3DVIEWPORT9 vp;
    pDevice->GetViewport(&vp);

    D3DXMATRIX proj;
    pDevice->GetTransform(D3DTS_PROJECTION, &proj);

    px = (((2.0f*x) / vp.Width) - 1.0f) / proj(0, 0);
    py = (((-2.0f*y) / vp.Height) + 1.0f) / proj(1, 1);

    d3d::Ray ray;
    ray._origin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    ray._direction = D3DXVECTOR3(px, py, 1.0f);

    return ray;
}

void d3d::TransformRay(d3d::Ray* ray, D3DXMATRIX* T)
{
    // transform the ray's origin, w = 1.
    D3DXVec3TransformCoord(
        &ray->_origin,
        &ray->_origin,
        T);

    // transform the ray's direction, w = 0.
    D3DXVec3TransformNormal(
        &ray->_direction,
        &ray->_direction,
        T);

    // normalize the direction
    D3DXVec3Normalize(&ray->_direction, &ray->_direction);
}

bool d3d::RaySphereIntTest(d3d::Ray* ray, d3d::BoundingSphere* sphere)
{
    D3DXVECTOR3 v = ray->_origin - sphere->_center;

    float b = 2.0f * D3DXVec3Dot(&ray->_direction, &v);
    float c = D3DXVec3Dot(&v, &v) - (sphere->_radius * sphere->_radius);

    // find the discriminant
    float discriminant = (b * b) - (4.0f * c);

    // test for imaginary number
    if (discriminant < 0.0f)
        return false;

    discriminant = sqrtf(discriminant);

    float s0 = (-b + discriminant) / 2.0f;
    float s1 = (-b - discriminant) / 2.0f;

    // if a solution is >= 0, then we intersected the sphere
    if (s0 >= 0.0f || s1 >= 0.0f)
        return true;

    return false;
}


float d3d::GetRandomFloat(float lowBound, float highBound)
{
    if (lowBound >= highBound) // bad input
        return lowBound;

    // get random float in [0, 1] interval
    float f = (rand() % 10000) * 0.0001f;

    // return float in [lowBound, highBound] interval. 
    return (f * (highBound - lowBound)) + lowBound;
}

void d3d::GetRandomVector(
    D3DXVECTOR3* out,
    D3DXVECTOR3* min,
    D3DXVECTOR3* max)
{
    out->x = GetRandomFloat(min->x, max->x);
    out->y = GetRandomFloat(min->y, max->y);
    out->z = GetRandomFloat(min->z, max->z);
}

DWORD d3d::FtoDw(float f)
{
    return *((DWORD*)&f);
}