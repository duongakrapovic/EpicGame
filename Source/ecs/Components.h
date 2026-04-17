#pragma once
#include "axmol.h"
#include <string>

// Vị trí
struct TransformComponent
{
    ax::Vec2 position;
};

// Sprite hiển thị
struct SpriteComponent
{
    ax::Sprite* sprite = nullptr;
};

// Input điều khiển
struct InputComponent
{
    bool up    = false;
    bool down  = false;
    bool left  = false;
    bool right = false;
    float speed = 0.0f;
};

// Animation
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
    float timer          = 0;
    int frame            = 0;
    bool moving          = false;
    std::string basePath;
};

// --- HỆ THỐNG HITBOX MỚI ---
enum class CollisionType
{
    None = 0,
    Player,
    Enemy,
    Item,
    Wall
};

struct CollisionComponent
{
    ax::Rect hitbox;  // Kích thước vùng va chạm (VD: 8x6)
    ax::Vec2 offset;  // Độ lệch so với tâm nhân vật (VD: chân)
    CollisionType type = CollisionType::None;
    bool isTrigger     = false;  // True nếu chỉ để nhặt đồ, không chặn di chuyển

    // Lấy hình chữ nhật va chạm thực tế trong thế giới game
    ax::Rect getWorldHitbox(const ax::Vec2& entityPos) const
    {
        return ax::Rect(entityPos.x + offset.x - hitbox.size.width / 2, entityPos.y + offset.y - hitbox.size.height / 2,
                        hitbox.size.width, hitbox.size.height);
    }
};
