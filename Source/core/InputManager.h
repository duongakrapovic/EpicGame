#pragma once
#include "axmol.h"

class InputManager
{
public:
    void setup(ax::Node* node);

    bool up    = false;
    bool down  = false;
    bool left  = false;
    bool right = false;
};
