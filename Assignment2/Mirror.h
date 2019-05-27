#pragma once
#include <d3dx9.h>
#include <vector>
#include "Mesh.h"
#include "d3dUtility.h"


//
// Classes and Structures
//
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
    Vertex(float x, float y, float z) {
        _x = x;  _y = y;  _z = z;
    }
    Vertex(const Vertex& vert,
        float nx, float ny, float nz,
        float u, float v) {
        _x = vert._x; _y = vert._y; _z = vert._z;
        _nx = nx; _ny = ny; _nz = nz;
        _u = u;  _v = v;
    }
    float _x, _y, _z;
    float _nx, _ny, _nz;
    float _u, _v;

    static const DWORD FVF;
};

class Mirror
{
    LPDIRECT3DDEVICE9 pDevice;
    D3DMATERIAL9 MirrorMtrl = d3d::WHITE_MTRL;
    IDirect3DVertexBuffer9* VB = 0;
    IDirect3DTexture9* MirrorTex = 0;
    Vertex faces[6];

public:
    bool setup(LPDIRECT3DDEVICE9 pDevice);
    void RenderMirror(std::vector<Mesh>* pmeshes, size_t size);
};
