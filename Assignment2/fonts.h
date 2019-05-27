#pragma once
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <string>

using namespace std;


class Fonts {
    ID3DXFont *font;
    RECT baseRect;
    D3DCOLOR baseColor;
    int font_height;
public:
    Fonts();
    ~Fonts();
    HRESULT Init(LPDIRECT3DDEVICE9 device, int height = 22, int width = 10, string name = "Arial");
    void setRect(int left, int top, int right, int bottom);
    void Print(string text, D3DCOLOR Color, RECT rect);
    void Print(string text, D3DCOLOR Color, int left, int top, int right, int bottom);
    void Print(string text, D3DCOLOR Color);
    void Print(string text);
};