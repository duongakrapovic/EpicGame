#pragma once
#include "world/World.h"
#include <cmath>

namespace Systems
{
// =========================
// MOVEMENT SYSTEM
// =========================
inline void Movement(World& w)
{
    for (auto& [e, input] : w.inputs)  // Chỉ lặp qua những thực thể có Input
    {
        auto& transform = w.transforms[e];
        ax::Vec2 dir(0, 0);

        if (input.up)
            dir.y += 1;
        if (input.down)
            dir.y -= 1;
        if (input.left)
            dir.x -= 1;
        if (input.right)
            dir.x += 1;

        auto& anim = w.animations[e];

        if (dir.length() > 0)
        {
            dir.normalize();
            transform.position += dir * 3.0f;
            anim.moving = true;

            if (std::abs(dir.x) > std::abs(dir.y))
                anim.currentDir = (dir.x > 0) ? AnimationComponent::RIGHT : AnimationComponent::LEFT;
            else
                anim.currentDir = (dir.y > 0) ? AnimationComponent::UP : AnimationComponent::DOWN;
        }
        else
        {
            anim.moving = false;
        }
    }
}

// =========================
// RENDER SYSTEM
// =========================
inline void Render(World& w)
{
    for (auto& [e, transform] : w.transforms)
    {
        if (w.sprites.count(e))
        {
            w.sprites[e].sprite->setPosition(transform.position);
        }
    }
}

// =========================
// ANIMATION SYSTEM
// =========================
inline std::string getPath(const std::string& base, int frame, AnimationComponent::Direction dir)
{
    // Cấu trúc mong muốn: "sprites/player/boy_down_1.png"
    std::string dirStr = "down";
    switch (dir)
    {
    case AnimationComponent::UP:
        dirStr = "up";
        break;
    case AnimationComponent::LEFT:
        dirStr = "left";
        break;
    case AnimationComponent::RIGHT:
        dirStr = "right";
        break;
    default:
        dirStr = "down";
        break;
    }

    // Tìm vị trí "_" cuối cùng (trước số 1) và "_" kế cuối (trước chữ down)
    size_t lastPos       = base.find_last_of('_');
    size_t secondLastPos = base.find_last_of('_', lastPos - 1);

    if (secondLastPos == std::string::npos)
        return base;  // Phòng lỗi chuỗi

    std::string prefix = base.substr(0, secondLastPos + 1);
    return prefix + dirStr + "_" + std::to_string(frame + 1) + ".png";
}

inline void Animation(World& w, float dt)
{
    for (auto& [e, anim] : w.animations)
    {
        if (!w.sprites.count(e))
            continue;
        auto& sprite = w.sprites[e];

        if (anim.moving)
        {
            anim.timer += dt;
            if (anim.timer > 0.2f)
            {
                anim.timer = 0;
                anim.frame = 1 - anim.frame;
                sprite.sprite->setTexture(getPath(anim.basePath, anim.frame, anim.currentDir));
            }
        }
        else
        {
            anim.frame = 0;
            sprite.sprite->setTexture(getPath(anim.basePath, anim.frame, anim.currentDir));
        }
    }
}
}  // namespace Systems
