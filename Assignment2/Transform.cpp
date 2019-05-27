#include "Transform.h"



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Default Constructor
No Parameters.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
Transform::Transform()
{
    D3DXMatrixIdentity(&TranslateMatrix);
    D3DXMatrixIdentity(&ScaleMatrix);
    D3DXMatrixIdentity(&RotationMatrix);
}

D3DXMATRIX Transform::GetTNet() {
    if (change) {
        TNet = ScaleMatrix * RotationMatrix * TranslateMatrix;
        boundingSphere._center.x = originalBoundingSphere._center.x + TNet._41 / TranslateMatrix._44;
        boundingSphere._center.y = originalBoundingSphere._center.y + TNet._42 / TranslateMatrix._44;
        boundingSphere._center.z = originalBoundingSphere._center.y + TNet._43 / TranslateMatrix._44;
        float scale = max(ScaleMatrix._11, max(ScaleMatrix._22, max(ScaleMatrix._33, ScaleMatrix._44)));
        boundingSphere._radius = originalBoundingSphere._radius * scale;
        change = false;
    }
    return TNet;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Absolute rotation
Parameters:
[in] x - X radians
[in] y - Y radians
[in] z - Z radians
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void Transform::SetRotation(float x, float y, float z)
{
    change = true;
    RotationX = x;
    RotationY = y;
    RotationZ = z;
    D3DXMatrixRotationYawPitchRoll(&RotationMatrix, y, x, z);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Relative rotation
Parameters:
[in] x - X radians
[in] y - Y radians
[in] z - Z radians
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void Transform::Rotate(float x, float y, float z)
{
    change = true;
    RotationX += x;
    RotationY += y;
    RotationZ += z;
    D3DXMatrixRotationYawPitchRoll(&RotationMatrix, RotationY, RotationX, RotationZ);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Absolute translation
Parameters:
[in] x - X units
[in] y - Y units
[in] z - Z units
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void Transform::SetPosition(float x, float y, float z)
{
    change = true;
    TranslateMatrix._41 = x;
    TranslateMatrix._42 = y;
    TranslateMatrix._43 = z;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Relative translation
Parameters:
[in] x - X units
[in] y - Y units
[in] z - Z units
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void Transform::Translate(float x, float y, float z)
{
    change = true;
    TranslateMatrix._41 += x;
    TranslateMatrix._42 += y;
    TranslateMatrix._43 += z;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Absolute scale
Parameters:
[in] x - X units
[in] y - Y units
[in] z - Z units
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void Transform::SetScale(float x, float y, float z)
{
    change = true;
    ScaleMatrix._11 = x;
    ScaleMatrix._22 = y;
    ScaleMatrix._33 = z;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Relative scale
Parameters:
[in] x - X units
[in] y - Y units
[in] z - Z units
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void Transform::Scale(float x, float y, float z)
{
    change = true;
    ScaleMatrix._11 *= x;
    ScaleMatrix._22 *= y;
    ScaleMatrix._33 *= z;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Resets the Translation, Rotation and Scale
No Parameters.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void Transform::Reset()
{
    change = true;
    SetPosition(0, 0, 0);
    SetRotation(0, 0, 0);
    SetScale(1, 1, 1);
}

