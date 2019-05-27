#pragma once


#include <d3d10.h>
#include <d3d9.h>
#include "Resource.h"
#include "d3dx9tex.h"
#include "window.h"
#include "fonts.h"
#include "Utils.h"
#include "Light.h"

class d3d
{
public:
    d3d() = default;
    ~d3d() = default;


    

	//
	// Colors
	//
    static const D3DXCOLOR      WHITE;
    static const D3DXCOLOR      BLACK;
    static const D3DXCOLOR        RED;
    static const D3DXCOLOR      GREEN;
    static const D3DXCOLOR       BLUE;
    static const D3DXCOLOR     YELLOW;
    static const D3DXCOLOR       CYAN;
    static const D3DXCOLOR    MAGENTA;
    //
    // Materials
    //

    static D3DMATERIAL9 InitMtrl(D3DXCOLOR a, D3DXCOLOR d, D3DXCOLOR s, D3DXCOLOR e, float p);

    static const D3DMATERIAL9 WHITE_MTRL;
    static const D3DMATERIAL9 RED_MTRL;
    static const D3DMATERIAL9 GREEN_MTRL;
    static const D3DMATERIAL9 BLUE_MTRL;
    static const D3DMATERIAL9 YELLOW_MTRL;


    LPDIRECT3DSURFACE9 pBackSurf;
    LPDIRECT3DSURFACE9 pSurface;
    LPDIRECT3D9 pD3D = 0;//COM object
    LPDIRECT3DDEVICE9 pDevice = 0;//graphics device

    Fonts fonts;
    Light light;


    int LoadBitmapToSurface(const char* FileName, LPDIRECT3DSURFACE9* ppSurface, LPDIRECT3DDEVICE9 pDevice);
    boolean Init(Window window, boolean fullScreen = true);
    boolean Init(HWND hWnd, int width = 1920, int height = 1080, boolean fullScreen = true);
    boolean Init3DWorld();




    //
    // Bounding Objects / Math Objects
    //

    struct BoundingBox
    {
        BoundingBox();

        bool isPointInside(D3DXVECTOR3& p);

        D3DXVECTOR3 _min;
        D3DXVECTOR3 _max;
    };

    struct BoundingSphere
    {
        BoundingSphere();

        D3DXVECTOR3 _center;
        float       _radius;
    };

    struct Ray
    {
        D3DXVECTOR3 _origin;
        D3DXVECTOR3 _direction;
    };

    //
    // Constants
    //

    static const float FLT_INFINITY;
    static const float EPSILON;

    struct Vertex
    {
        Vertex() {}
        Vertex(float x, float y, float z,
            float nx, float ny, float nz,
            float u, float v)
        {
            _x = x;  _y = y;  _z = z;
            _nx = nx; _ny = ny; _nz = nz;
            _u = u;  _v = v;
        }
        float _x, _y, _z;
        float _nx, _ny, _nz;
        float _u, _v;

        static const DWORD FVF;
    };


    d3d::Ray CalcPickingRay(int x, int y);
    void TransformRay(d3d::Ray* ray, D3DXMATRIX* T);
    bool d3d::RaySphereIntTest(d3d::Ray* ray, d3d::BoundingSphere* sphere);



    //
    // Randomness
    //

    // Desc: Return random float in [lowBound, highBound] interval.
    static float GetRandomFloat(float lowBound, float highBound);


    // Desc: Returns a random vector in the bounds specified by min and max.
    static void GetRandomVector(
        D3DXVECTOR3* out,
        D3DXVECTOR3* min,
        D3DXVECTOR3* max);

    //
    // Conversion
    //
    static DWORD FtoDw(float f);

};


int InitDirect3DDevice(HWND hWndTarget, int Width, int Height, BOOL bWindowed, D3DFORMAT FullScreenFormat, LPDIRECT3D9 pD3D, LPDIRECT3DDEVICE9* ppDevice);