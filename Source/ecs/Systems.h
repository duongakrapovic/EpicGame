#pragma once
#include "world/World.h"
#include <cmath>

namespace Systems
{
// =========================
// MOVEMENT SYSTEM
// =========================
// Trong ecs/Systems.h -> namespace Systems

inline void Movement(World& w)
{
    for (auto& [e, input] : w.inputs)
    {
        auto& transform = w.transforms[e];
        auto& anim      = w.animations[e];
        ax::Vec2 moveVec(0, 0);

        // 1. Thu thập hướng di chuyển từ input
        if (input.up)
            moveVec.y += 1;
        if (input.down)
            moveVec.y -= 1;
        if (input.left)
            moveVec.x -= 1;
        if (input.right)
            moveVec.x += 1;

        if (moveVec.length() > 0)
        {
            // 2. Chuẩn hóa vector để di chuyển chéo không bị nhanh hơn
            moveVec.normalize();

            // 3. Tính toán vận tốc dựa trên speed từ JSON (chia 60 để khớp FPS)
            // Ví dụ speed = 120 -> 2 pixel mỗi frame
            float currentSpeed = input.speed / 60.0f;
            ax::Vec2 velocity  = moveVec * currentSpeed;

            // 4. Xử lý va chạm (Collision)
            if (w.collisions.count(e))
            {
                auto& col = w.collisions[e];
                if (!col.isTrigger)
                {
                    // Kiểm tra va chạm trục X độc lập
                    ax::Vec2 nextPosX = transform.position + ax::Vec2(velocity.x, 0);
                    if (!w.map.isCollision(col.getWorldHitbox(nextPosX)))
                    {
                        transform.position.x = nextPosX.x;
                    }

                    // Kiểm tra va chạm trục Y độc lập
                    ax::Vec2 nextPosY = transform.position + ax::Vec2(0, velocity.y);
                    if (!w.map.isCollision(col.getWorldHitbox(nextPosY)))
                    {
                        transform.position.y = nextPosY.y;
                    }
                }
                else
                {
                    // Nếu là Trigger (xuyên thấu) thì cộng thẳng vị trí
                    transform.position += velocity;
                }
            }
            else
            {
                // Nếu không có thành phần collision
                transform.position += velocity;
            }

            // 5. Cập nhật trạng thái Animation
            anim.moving = true;
            if (std::abs(moveVec.x) > std::abs(moveVec.y))
                anim.currentDir = (moveVec.x > 0) ? AnimationComponent::RIGHT : AnimationComponent::LEFT;
            else
                anim.currentDir = (moveVec.y > 0) ? AnimationComponent::UP : AnimationComponent::DOWN;
        }
        else
        {
            // Đứng yên
            anim.moving = false;
        }
    }
}
// =========================
// RENDER SYSTEM
// =========================
static ax::DrawNode* debugDraw = nullptr;

inline void Render(World& w)
{
    if (!debugDraw)
    {
        debugDraw = ax::DrawNode::create();
        if (w.worldNode)
            w.worldNode->addChild(debugDraw, 1000);
    }
    debugDraw->clear();

    for (auto& [e, transform] : w.transforms)
    {
        // LÀM TRÒN TỌA ĐỘ: Cực kỳ quan trọng để khung đỏ không trôi lệch
        float rx = std::round(transform.position.x);
        float ry = std::round(transform.position.y);

        if (w.sprites.count(e))
        {
            w.sprites[e].sprite->setPosition(ax::Vec2(rx, ry));
        }

        if (w.collisions.count(e))
        {
            auto& col = w.collisions[e];
            // Vẽ dựa trên tọa độ đã làm tròn (rx, ry)
            ax::Rect hb = col.getWorldHitbox(ax::Vec2(rx, ry));
            debugDraw->drawRect(hb.origin, hb.origin + hb.size, ax::Color4F::RED);
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

        bool frameChanged = false;

        if (anim.moving)
        {
            anim.timer += dt;
            if (anim.timer > 0.2f)
            {
                anim.timer   = 0;
                anim.frame   = 1 - anim.frame;
                frameChanged = true;
            }
        }
        else
        {
            if (anim.frame != 0)
            {
                anim.frame   = 0;
                frameChanged = true;
            }
        }

        // Nếu hướng thay đổi cũng coi như frame thay đổi
        // (Logic đơn giản hóa để luôn đảm bảo texture được set Alias)

        std::string newPath = getPath(anim.basePath, anim.frame, anim.currentDir);

        // Luôn kiểm tra và cập nhật texture
        auto texture = ax::Director::getInstance()->getTextureCache()->addImage(newPath);
        if (texture)
        {
            texture->setAliasTexParameters();  // Buộc ảnh mới phải sắc nét
            sprite.sprite->setTexture(texture);
        }
    }
}
}  // namespace Systems
