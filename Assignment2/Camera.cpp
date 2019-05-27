#include "Camera.h"


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Constructor
Parameters:
[in] pDevice - D3D Device
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
Camera::Camera()
{
    UpdateCamera();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Sets the position of the camera
Parameters:
[in] eye - A transform that will be the origin of the camera
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void Camera::SetPosition(Transform* eye) {
    SetPosition(eye->GetPosition());
    CameraEye = eye;
    HasCameraEye = true;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Sets the position of the camera
Parameters:
[in] position - A position matrix that will be the origin of the camera
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void Camera::SetPosition(D3DXVECTOR3 position) {
    HasCameraEye = false;
    Position = position;
    UpdateCamera();
}

void Camera::GetPosition(D3DXVECTOR3 *position) {
    *position = Position;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Translates the camera
Parameters:
[in] vector - direction of the translation
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void Camera::Translate(D3DXVECTOR3 vector) {
    HasCameraEye = false;
    Position += vector;
    if (!HasLookAtTarget && !LookAtLocked) {
        LookAtPosition += vector;
    }
    UpdateCamera();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Sets where the camera is looking at
Parameters:
[in] target - A transform that will be what the camera is looking at
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void Camera::LookAt(Transform* target) {
    LookAt(target->GetPosition());
    LookAtTarget = target;
    HasLookAtTarget = true;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Sets where the camera is looking at
Parameters:
[in] position - A position matrix that will be what the camera is looking at
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void Camera::LookAt(D3DXVECTOR3 position) {
    LookAtLocked = false;
    HasLookAtTarget = false;
    LookAtPosition = position;
    UpdateCamera();
}


void Camera::GetLook(D3DXVECTOR3* look)
{
    D3DXVECTOR3 lookDir = LookAtPosition - Position;
    *look = lookDir;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Translates the position the camera is looking at
Parameters:
[in] vector - direction of the translation
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void Camera::TranslateLookAt(D3DXVECTOR3 vector) {
    HasCameraEye = false;
    LookAtPosition += vector;
    UpdateCamera();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Locks the position of where the camera is looking at
No Parameters.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void Camera::LockTargetPosition() {
    HasLookAtTarget = false;
    LookAtLocked = true;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Unlocks the position of where the camera is looking at
No Parameters.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void Camera::UnlockTargetPosition() {
    LookAtLocked = false;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Removes a reference transform in case the matrix is using it as origin or as target
Parameters:
[in] transform - being removed from the scene
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void Camera::RemoveReference(Transform* transform) {
    if (HasLookAtTarget && LookAtTarget == transform) {
        LookAtTarget = 0;
        HasLookAtTarget = false;
    }
    if (HasCameraEye && CameraEye == transform) {
        CameraEye = 0;
        HasCameraEye = false;
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Updates the matrices of the camera if needed
No Parameters.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void Camera::Update() {
    if (HasLookAtTarget || HasCameraEye) {
        if ((HasLookAtTarget && LookAtPosition != LookAtTarget->GetPosition())
            || (HasCameraEye && Position != CameraEye->GetPosition())) {
            if (HasLookAtTarget)
                LookAtPosition = LookAtTarget->GetPosition();
            if (HasCameraEye)
                Position = CameraEye->GetPosition();
            UpdateCamera();
        }
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Forces update on the matrices of the camera
No Parameters.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void Camera::UpdateCamera() {
    viewMatrix;
    D3DXMatrixLookAtLH(&viewMatrix, &Position, &LookAtPosition, &CameraUp);
    D3DXMatrixPerspectiveFovLH(&projection, D3DX_PI / 3.0f, 1.0f, 1.0f, 1000.0f);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Applies the camera view to the device
Parameters:
[in] pDevice - pointer to the device to receive the camera view
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void Camera::Apply(LPDIRECT3DDEVICE9 pDevice) {
    Update();
    pDevice->SetTransform(D3DTS_VIEW, &viewMatrix);
    pDevice->SetTransform(D3DTS_PROJECTION, &projection);
}