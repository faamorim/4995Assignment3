#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include "Transform.h"
#include <vector>
#include "Utils.h"
#include "d3dUtility.h"

class Mesh : public Transform
{
public:
    Mesh() {};
    Mesh(LPDIRECT3DDEVICE9 pDevice);
    ~Mesh();
    int GetNumMaterials() { return Materials.size(); }
    LPD3DXMESH GetMesh() { return pMesh; }
    D3DMATERIAL9* GetMaterial(int i) { return &(Materials[i]); }
    LPDIRECT3DTEXTURE9 GetTexture(int i) { return Textures[i]; }

    void Render();
    void Render(D3DXMATRIX* T, D3DXMATRIX* R);
    bool Load(char* folder, char* file);
    void SetDevice(LPDIRECT3DDEVICE9 pDevice) { this->pDevice = pDevice; }



private:
    LPDIRECT3DDEVICE9 pDevice = 0;
    LPD3DXMESH pMesh = 0;
    std::vector<D3DMATERIAL9> Materials;
    std::vector<LPDIRECT3DTEXTURE9> Textures;
};

