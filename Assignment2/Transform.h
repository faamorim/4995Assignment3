#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include "d3dUtility.h"


class Transform
{
public:
    Transform();
    ~Transform() {};

    d3d::BoundingSphere boundingSphere;
    d3d::BoundingSphere originalBoundingSphere;
    ID3DXMesh* boundingSphereMesh = 0;

    bool change = true;

    D3DXMATRIX GetTNet();

    D3DXMATRIX* GetTransform() { GetTNet(); return &TNet; }

    D3DXVECTOR3 GetPosition() { return D3DXVECTOR3(TranslateMatrix._41, TranslateMatrix._42, TranslateMatrix._43); }
    D3DXVECTOR3 GetScale() { return D3DXVECTOR3(ScaleMatrix._11, ScaleMatrix._22, ScaleMatrix._33); }
    D3DXVECTOR3 GetRotation() { return D3DXVECTOR3(RotationX, RotationY, RotationZ); }
    void SetPosition(float x, float y, float z);
    void SetPosition(D3DXVECTOR3 pos) { SetPosition(pos.x, pos.y, pos.z); }
    void Translate(float x, float y, float z);
    void Translate(D3DXVECTOR3 pos) { Translate(pos.x, pos.y, pos.z); }
    void Scale(float x, float y, float z);
    void Scale(D3DXVECTOR3 pos) { Scale(pos.x, pos.y, pos.z); }
    void SetScale(float x, float y, float z);
    void SetScale(D3DXVECTOR3 pos) { SetScale(pos.x, pos.y, pos.z); }
    void Rotate(float x, float y, float z);
    void Rotate(D3DXVECTOR3 pos) { Rotate(pos.x, pos.y, pos.z); }
    void SetRotation(float x, float y, float z);
    void SetRotation(D3DXVECTOR3 pos) { SetRotation(pos.x, pos.y, pos.z); }
    void Reset();

private:
    D3DXMATRIX TranslateMatrix;
    D3DXMATRIX RotationMatrix;
    D3DXMATRIX ScaleMatrix;
    D3DXMATRIX TNet;
    float RotationX = 0;
    float RotationY = 0;
    float RotationZ = 0;
};

