#pragma once
#include "axmol.h"

class CameraController
{
public:
    void follow(ax::Node* worldNode, ax::Node* target);
};
