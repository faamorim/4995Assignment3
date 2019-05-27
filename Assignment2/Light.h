#pragma once

#include <d3d9.h>
#include <d3dx9.h>


class Light
{

    LPDIRECT3DDEVICE9 pDevice = 0;//graphics device

    void SetupLights();
public:
    Light() = default;
    Light(LPDIRECT3DDEVICE9 pDevice) : pDevice(pDevice) { SetupLights(); };
    ~Light() = default;


    void SetLight(int i);
};

