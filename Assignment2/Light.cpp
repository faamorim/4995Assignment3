#include "Light.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Sets up the different Lights
No Parameters.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void Light::SetupLights() {
    D3DLIGHT9 light;    // create the light struct
    int index = 0;
    //First Light: Directional
    ZeroMemory(&light, sizeof(light));    // clear out the light struct for use
    light.Type = D3DLIGHT_DIRECTIONAL;    // set the light type
    light.Diffuse = D3DXCOLOR(1.0f, 0.8f, 0.5f, 1.0f);    // set the light's color
    light.Direction = D3DXVECTOR3(-1.0f, -0.3f, -1.0f);

    pDevice->SetLight(index++, &light);    // send the light struct properties to light #0

    //Second Light: Spot
    ZeroMemory(&light, sizeof(light));    // clear out the light struct for use
    light.Type = D3DLIGHT_SPOT;    // set the light type
    light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);    // set the light's color
    light.Position = D3DXVECTOR3(0.0f, 15.0f, 0.0f);
    light.Direction = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
    light.Theta = 0.1f;
    light.Phi = 0.5f;
    light.Falloff = 1.0f;
    light.Range = 500.0f;


    pDevice->SetLight(index++, &light);    // send the light struct properties to light #0

    //Third Light: Point
    ZeroMemory(&light, sizeof(light));    // clear out the light struct for use
    light.Type = D3DLIGHT_POINT;    // set the light type
    light.Diffuse = D3DXCOLOR(0.7f, 0.2f, 1.0f, 0.0f);    // set the light's color
    light.Position = D3DXVECTOR3(2.0f, 2.0f, -3.0f);
    light.Attenuation1 = 0.5f;
    light.Range = 500.0f;


    pDevice->SetLight(index++, &light);    // send the light struct properties to light #0




    //Forth Light: Spot
    ZeroMemory(&light, sizeof(light));    // clear out the light struct for use
    light.Type = D3DLIGHT_SPOT;    // set the light type
    light.Diffuse = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);    // set the light's color
    light.Position = D3DXVECTOR3(-10.0f, 15.0f, -10.0f);
    light.Direction = D3DXVECTOR3(10.0f, -15.0f, 10.0f);
    light.Theta = 0.05f;
    light.Phi = 0.25f;
    light.Falloff = 1.0f;
    light.Range = 500.0f;
    pDevice->SetLight(index++, &light);    // send the light struct properties to light #0
    //Forth Light: Spot
    ZeroMemory(&light, sizeof(light));    // clear out the light struct for use
    light.Type = D3DLIGHT_SPOT;    // set the light type
    light.Diffuse = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);    // set the light's color
    light.Position = D3DXVECTOR3(-10.0f, 15.0f, 10.0f);
    light.Direction = D3DXVECTOR3(10.0f, -15.0f, -10.0f);
    light.Theta = 0.05f;
    light.Phi = 0.25f;
    light.Falloff = 1.0f;
    light.Range = 500.0f;
    pDevice->SetLight(index++, &light);    // send the light struct properties to light #0
    //Forth Light: Spot
    ZeroMemory(&light, sizeof(light));    // clear out the light struct for use
    light.Type = D3DLIGHT_SPOT;    // set the light type
    light.Diffuse = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);    // set the light's color
    light.Position = D3DXVECTOR3(15.0f, 15.0f, 0.0f);
    light.Direction = D3DXVECTOR3(-15.0f, -15.0f, 0.0f);
    light.Theta = 0.05f;
    light.Phi = 0.25f;
    light.Falloff = 1.0f;
    light.Range = 500.0f;
    pDevice->SetLight(index++, &light);    // send the light struct properties to light #0




    SetLight(2);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Activates the specified light
Parameters:
[in] i - index of the light to activate
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void Light::SetLight(int i) {
    if (i == 1) {
        pDevice->SetRenderState(D3DRS_AMBIENT, 0xffffffff);
    }
    else {
        pDevice->SetRenderState(D3DRS_AMBIENT, 0x00000000);
    }
    for (int l = 0; l < 6; l++) {
        if (l == i-2) {
            pDevice->LightEnable(l, TRUE);    // turn on light
        }
        else {
            pDevice->LightEnable(l, FALSE);    // turn off light
        }
    }
    if (i == 8) {
        pDevice->LightEnable(3, TRUE);    // turn on light
        pDevice->LightEnable(4, TRUE);    // turn on light
        pDevice->LightEnable(5, TRUE);    // turn on light
    }
    if (i == 9) {

    }
}