#include "game.h"


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Default Constructor
No Parameters.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
Game::Game() {

}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Default Destructor
No Parameters.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
Game::~Game()
{
    if (pDevice)
        pDevice->Release();

    if (pD3D)
        pD3D->Release();
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Calculates the framerate of the application
No Parameters.
Returns the framerate
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int Game::countFrameRate() {
    tStart = tEnd;
    QueryPerformanceCounter(&tEnd);
    int dif = (int)(tEnd.QuadPart - tStart.QuadPart);
    if (dif > 0) {
        frameTime = dif / frameMult;
        return (int)(frameMult / dif);
    }
    return 0;
}








/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Tries to create a Mesh object and adds it to the scene.
Parameters:
[in] folder - the folder path of the file
[in] fileName - file name
Returns succes or fail
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
boolean Game::AddMesh(char* folder, char* fileName) {
    int index = meshes.size();
    meshes.push_back(Mesh{ pDevice });
    if (!meshes[index].Load(folder, fileName)) {
        meshes.erase(meshes.begin() + index);
        return false;
    }
    return true;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Starts the Game Loop
No Parameters.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void Game::Start() {
    MSG msg;
    //
    // Create the Firework system.
    //
    D3DXVECTOR3 origin(0.0f, 6.0f, 0.0f);
    mirror.setup(pDevice);
    while (TRUE) {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT)
                break;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else {
            GameLoop();
        }
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Game Loop that reads input and renders the scene
No Parameters.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int Game::GameLoop() {
    ReadCommand();

    frameRate = countFrameRate();

    if (Exp != 0) {
        Exp->update(frameTime / 10.0f);
    }
    Render();

    return S_OK;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Reads, interprets and executes the Keyboard Input if possible, calling Apply Command if needed
Parameters:
[in] keyevent - the key event that triggered the message
[in] keycode - code that represents the key
Returns 0
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
WORD Game::ReadKeyboardInput(UINT keyevent, WPARAM keycode) {
    float value = 1;
    switch (keyevent) {
    case WM_KEYUP:
        switch (keycode) {
        case VK_CONTROL:
            ctrl = false;
            break;
        case VK_SHIFT:
            shift = false;
            break;
        case VK_MENU:
            alt = false;
            break;
        case VK_OEM_MINUS:
            minus = false;
            break;
        case VK_OEM_PLUS:
            plus = false;
            break;
        }
        break;
    case WM_KEYDOWN:
        switch (keycode) {
        case VK_ESCAPE:
            PostQuitMessage(0);
            break;
        case VK_CONTROL:
            ctrl = true;
            break;
        case VK_SHIFT:
            shift = true;
            break;
        case VK_MENU:
            alt = true;
            break;
        case VK_INSERT: 
            if (shift) {
                AddMesh("Meshes/Airplane/", "airplane 2.x");
            }
            else if (ctrl) {
                AddMesh("Meshes/Dwarf/", "Dwarf.x");
            }
            else {
                AddMesh("Meshes/Tiger/", "tiger.x");
            }
            selectedMeshIndex = meshes.size() - 1;
            break;
        case VK_DELETE:
            if (meshes.size() > 0) {
                selectedMeshIndex %= meshes.size();
                if (Exp!=0) {
                }
                D3DXVECTOR3 origin(0.0f, 6.0f, 0.0f);
                origin = meshes[selectedMeshIndex].GetPosition();
                Exp = new psys::Firework(&origin, 600);
                Exp->init(pDevice, "flare.bmp");
                camera.RemoveReference(&(meshes[selectedMeshIndex]));
                meshes.erase(meshes.begin() + selectedMeshIndex);
                selectedMeshIndex--;
            }
            break;
        case 0x42:
            PrintingBackground = !PrintingBackground;
            break;
        case VK_OEM_COMMA:
            value = -1;
        case VK_OEM_PERIOD:
            if (ChangingCameraPosition || ChangingCameraTarget) {
                ChangingCameraPosition = false;
                ChangingCameraTarget = false;
            }
            else if (meshes.size() > 0){
                selectedMeshIndex += value;
                selectedMeshIndex %= meshes.size();
            }
            break;
        case VK_SPACE:
            if (meshes.size() > 0) {
                selectedMeshIndex %= meshes.size();
                camera.LookAt(&meshes[selectedMeshIndex]);
            }
            break;


        case 0x43: // C
            ChangingCameraPosition = true;
            ChangingCameraTarget = false;
            break;
        case 0x46: // F
            ChangingCameraPosition = false;
            ChangingCameraTarget = true;
            break;
        case 0x4C: // L
            camera.LockTargetPosition();
            break;
        case 0x4B: // K
            camera.UnlockTargetPosition();
            break;

        case 0x54: // T
            if (!selectedAxis)
                selectedAxis = oldSelectedAxis;
            selectedCommand = COMMAND_TRANSLATE;
            break;
        case 0x52: // R
            if (!selectedAxis)
                selectedAxis = oldSelectedAxis;
            selectedCommand = COMMAND_ROTATE;
            break;
        case 0x53: // S
            selectedCommand = COMMAND_SCALE;
            if (selectedAxis)
                oldSelectedAxis = selectedAxis;
            selectedAxis = 0;
            break;
        case 0x58: // X
            selectedAxis = AXIS_X;
            break;
        case 0x59: // Y
            selectedAxis = AXIS_Y;
            break;
        case 0x5A: // Z
            selectedAxis = AXIS_Z;
            break;
        case 0x39:
            value++;
        case 0x38:
            value++;
        case 0x37:
            value++;
        case 0x36:
            value++;
        case 0x35:
            value++;
        case 0x34:
            value++;
        case 0x33:
            value++;
        case 0x32:
            value++;
        case 0x31:
            value++;
        case 0x30:
            light.SetLight(value);
            break;
        case VK_OEM_MINUS:
            minus = true;
            break;
        case VK_OEM_PLUS:
            plus = true;
            break;
        }
        break;
    }
    return 0;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Reads, interprets and executes the Mouse Input if possible, calling Apply Command if needed
Parameters:
[in] mouseevent - the mouse event that triggered the message
[in] mousex - the X position of the mouse
[in] mousey - the Y position of the mouse
Returns 0
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
WORD Game::ReadMouseInput(UINT mouseevent, WORD mousex, WORD mousey) {
    if (meshes.size() <= 0) {
        return 0;
    }
    // compute the ray in view space given the clicked screen point
    d3d::Ray ray = CalcPickingRay(mousex, mousey);

    // transform the ray to world space
    D3DXMATRIX view;
    pDevice->GetTransform(D3DTS_VIEW, &view);

    D3DXMATRIX viewInverse;
    D3DXMatrixInverse(&viewInverse, 0, &view);

    TransformRay(&ray, &viewInverse);

    // test for a hit
    for (int i = 0; i < meshes.size(); i++) {
        if (RaySphereIntTest(&ray, &(meshes[i].boundingSphere))) {
            selectedMeshIndex = i;
            return 0;
        }
    }
    return 0;
}

int Game::ReadCommand() {
    if (meshes.size() <= 0 && !ChangingCameraPosition && !ChangingCameraTarget) {
        return 0;
    }

    float dir = 0;

    if (minus) {
        dir--;
    }
    if (plus) {
        dir++;
    }
    if (!dir) {
        return 0;
    }

    if (shift) {
        dir *= 5;
    }
    else if (ctrl) {
        dir /= 5;
    }


    if (dir) {
        ApplyCommand(dir);
    }


    return 0;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Applies a Transform command (translate, rotate or scale)
Parameters:
[in] direction - the direction of the command
Returns 0
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int Game::ApplyCommand(float direction) {
    float value = frameTime;
    D3DXVECTOR3 transformVector{ 0,0,0 };
    if (selectedAxis == AXIS_X) {
        transformVector.x = direction;
    }
    else if (selectedAxis == AXIS_Y) {
        transformVector.y = direction;
    }
    else if (selectedAxis == AXIS_Z) {
        transformVector.z = direction;
    }
    else if (!ChangingCameraPosition && !ChangingCameraTarget && selectedCommand == COMMAND_SCALE) {
        transformVector += D3DXVECTOR3{ 1,1,1 };
        transformVector *= direction;
    }
    
    transformVector *= value;

    if (ChangingCameraPosition) {
        camera.Translate(transformVector);
    }
    else if (ChangingCameraTarget) {
        camera.TranslateLookAt(transformVector);
    }
    else if (selectedCommand == COMMAND_TRANSLATE) {
        meshes[selectedMeshIndex].Translate(transformVector);
    }
    else if (selectedCommand == COMMAND_SCALE) {
        transformVector *= 0.01;
        transformVector += D3DXVECTOR3{ 1,1,1 };
        meshes[selectedMeshIndex].Scale(transformVector);
    }
    else if (selectedCommand == COMMAND_ROTATE) {
        transformVector *= 0.314;
        meshes[selectedMeshIndex].Rotate(transformVector);
    }


    return 0;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Renders the scene, background, 3D objecs and HUD
No Parameters.
Returns succes or fail
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int Game::Render() {
    HRESULT r;
    pDevice->Clear(0, 0,
        D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,
        0xff000000, 1.0f, 0L);
    pDevice->Clear(0, 0,
        D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
        D3DCOLOR_XRGB(0, 0, 25), 1.0f, 0);

    if(PrintingBackground)
        PrintBackground();

    pDevice->BeginScene();
    camera.Apply(pDevice);

    
    for (int i = 0; i < meshes.size(); i++) {
        meshes[i].Render();
    }

    mirror.RenderMirror(&meshes, meshes.size());

    D3DXMATRIX I;
    D3DXMatrixIdentity(&I);
    pDevice->SetTransform(D3DTS_WORLD, &I);
    if (Exp != 0) {
        Exp->render();
    }

    PrintHUD();
    pDevice->EndScene();
    pDevice->Present(NULL, NULL, NULL, NULL);//swap over buffer to primary surface
    return S_OK;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Prints the background to the screen
No Parameters.
Returns succes or fail
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int Game::PrintBackground() {
    HRESULT r;
    pBackSurf = 0;
    if (!pDevice) {
        SetError("Cannot render because there is no device");
        return E_FAIL;
    }

    //clear the display arera with colour black, ignore stencil buffer

    //get pointer to backbuffer
    r = pDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackSurf);
    if (FAILED(r)) {
        SetError("Couldn't get backbuffer");
    }

    r = pDevice->UpdateSurface(pSurface, NULL, pBackSurf, NULL);
    if (FAILED(r))
        SetError("did not copy surface");

    pBackSurf->Release();//release lock
}




/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Prints the user interface (fps counter)
No Parameters.
Returns succes or fail
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int Game::PrintHUD() {
    string fps = to_string(frameRate);
    string displayString = fps + "fps";
    fonts.Print(displayString);
    return S_OK;
}


