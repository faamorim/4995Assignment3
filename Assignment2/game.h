#pragma once
//#define D3D10_IGNORE_SDK_LAYERS

#define COMMAND_TRANSLATE 10001
#define COMMAND_ROTATE 10002
#define COMMAND_SCALE 10003
#define AXIS_X 20001
#define AXIS_Y 20002
#define AXIS_Z 20003

#include <d3d10.h>
#include <d3d9.h>
#include "Resource.h"
#include "d3dx9tex.h"
#include "window.h"
#include "fonts.h"
#include <stdio.h>
#include <tchar.h>
#include "Mesh.h"
#include "Camera.h"
#include <string>
#include <vector>
#include "Utils.h"
#include "Light.h"
#include "InputReader.h"
#include "d3dUtility.h"
#include "Mirror.h"
#include "pSystem.h"

class Game : public InputReader, public d3d
{
public:
    float frameMult = 1000000.0f;
    psys::PSystem* Exp = 0;
    Mirror mirror;
    LARGE_INTEGER tStart;
    LARGE_INTEGER tEnd;
    int PrintBackground();
    int PrintHUD();
    int frameRate;
    float frameTime;
    int countFrameRate();
    std::vector<Mesh> meshes;
    Camera camera;
    int Render();


    virtual WORD ReadKeyboardInput(UINT, WPARAM) override;
    virtual WORD ReadMouseInput(UINT, WORD, WORD) override;

    bool minus = false;
    bool plus = false;
    int ReadCommand();
    int ApplyCommand(float direction);
    boolean PrintingBackground = false;
    boolean ChangingPrintBackground = false;
    boolean SelectingMesh = false;
    boolean AddOrRemoveMesh = false;
    boolean ChangingCameraPosition = false;
    boolean ChangingCameraTarget = false;
    int selectedCommand = COMMAND_TRANSLATE;
    int oldSelectedAxis = 0;
    int selectedAxis = AXIS_X;
    int selectedMeshIndex = 0;
    boolean AddMesh(char* folder, char* fileName);

public:
    Game();
    ~Game();
    int GameLoop();
    void Start();
};


