#pragma once

#include <Windows.h>
class InputReader {
public:
    bool shift = false;
    bool ctrl = false;
    bool alt = false;
    virtual WORD ReadKeyboardInput(UINT, WPARAM) = 0;
    virtual WORD ReadMouseInput(UINT, WORD, WORD) = 0;
};