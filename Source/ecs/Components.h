#pragma once
#include "axmol.h"

//  Vị trí
struct Transform
{
    ax::Vec2 position;
};

//  Sprite hiển thị
struct SpriteComponent
{
    ax::Sprite* sprite = nullptr;
};

// 🔥 Input
struct InputComponent
{
    bool up    = false;
    bool down  = false;
    bool left  = false;
    bool right = false;
};

// 🔥 Animation
struct AnimationComponent
{
    enum Direction
    {
        DOWN,
        UP,
        LEFT,
        RIGHT
    };

    Direction currentDir = DOWN;

    float timer = 0;
    int frame   = 0;
    bool moving = false;
    std::string basePath;
};

struct TransformComponent
{
    ax::Vec2 position;
};
