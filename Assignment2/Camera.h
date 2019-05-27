#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include "Transform.h"
class Camera
{
public:
    Camera();
    ~Camera() = default;

    void SetPosition(Transform* eye);
    void SetPosition(D3DXVECTOR3 position);
    void SetPosition(float x, float y, float z) { SetPosition(D3DXVECTOR3(x, y, z)); }
    void GetPosition(D3DXVECTOR3* pos);
    void Translate(D3DXVECTOR3 vector);
    void Translate(float x, float y, float z) { Translate(D3DXVECTOR3(x, y, z)); }
    void LookAt(Transform* target);
    void LookAt(D3DXVECTOR3 position);
    void LookAt(float x, float y, float z) { LookAt(D3DXVECTOR3(x, y, z)); }
    void GetLook(D3DXVECTOR3* look);
    void TranslateLookAt(D3DXVECTOR3 position);
    void TranslateLookAt(float x, float y, float z) { TranslateLookAt(D3DXVECTOR3(x, y, z)); }
    void LockTargetPosition();
    void UnlockTargetPosition();
    void LookAtLock(D3DXVECTOR3 position) { LookAt(position); LockTargetPosition(); }
    void LookAtLock(float x, float y, float z) { LookAtLock(D3DXVECTOR3(x, y, z)); }
    void RemoveReference(Transform* transform);
    void Update();
    void Apply(LPDIRECT3DDEVICE9 pDevice);

private:
    void UpdateCamera();
    bool HasLookAtTarget = false;
    bool LookAtLocked = false;
    bool HasCameraEye = false;
    Transform* LookAtTarget;
    Transform* CameraEye;
    D3DXVECTOR3 LookAtPosition = { 0.0f, 0.0f, 0.0f };
    D3DXVECTOR3 CameraUp = { 0.0f, 1.0f, 0.0f };
    D3DXVECTOR3 Position = {0.0f, 8.0f, -15.0f};
    D3DXMATRIX viewMatrix;
    D3DXMATRIXA16 projection;
};

