#include "fonts.h"

Fonts::Fonts(){

}

HRESULT Fonts::Init(LPDIRECT3DDEVICE9 device, int height, int width, string name) {
    HRESULT hr = D3DXCreateFont(device,     //D3D Device
        height,               //Font height
        width,                //Font width
        FW_NORMAL,        //Font Weight
        1,                //MipLevels
        false,            //Italic
        DEFAULT_CHARSET,  //CharSet
        OUT_DEFAULT_PRECIS, //OutputPrecision
        ANTIALIASED_QUALITY, //Quality
        DEFAULT_PITCH | FF_DONTCARE,//PitchAndFamily
        name.c_str(),          //pFacename,
        &font);         //ppFont
    setRect(50, 50, 350, 250);
    baseColor = 0xFFFFFFFF;
    return hr;
}

Fonts::~Fonts() {
    if (font)
        font->Release();
}

void Fonts::setRect(int left, int top, int right, int bottom) {
    baseRect.left = left;
    baseRect.top = top;
    baseRect.right = right;
    baseRect.bottom = bottom;
}


void Fonts::Print(string text, D3DCOLOR color, RECT rect) {
    font_height = font->DrawText(NULL,        //pSprite
        text.c_str(),      //pString
        -1,          //Count
        &rect,  //pRect
        DT_LEFT | DT_WORDBREAK, //Format,
        color); //Color
    if (!font_height) {

    }
}

void Fonts::Print(string text, D3DCOLOR color, int left, int top, int right, int bottom) {
    RECT rect;
    rect.left = left;
    rect.top = top;
    rect.right = right;
    rect.bottom = bottom;
    Print(text, color, rect);
}

void Fonts::Print(string text, D3DCOLOR color) {
    Print(text, color, baseRect);
}

void Fonts::Print(string text) {
    Print(text, baseColor, baseRect);
}

