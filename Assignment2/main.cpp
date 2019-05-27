#define WIN32_LEAN_AND_MEAN

#include "game.h"
#include <Windows.h>
#include <mmsystem.h>
#include <d3dx9.h>

#include "Mesh.h"
#include "game.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Main entry point of the program. Called by the system
Parameters:
A lot of stuff that we don't actually care...
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pstrCmdLine, int iCmdShow){
    Game game;
    Application app{ "Assignment 2", hInstance};
    Window window{ app, 512, 512 , &game};
    window.Show(iCmdShow);
    if (game.Init(window, false)) {
        game.Init3DWorld();
        game.Start();
    }
	return 1;
}



