#include "MovementSystem.h"
#include <cmath>
namespace Systems
{
    void Movement(World& w)
    {
        for (auto& [e, input] : w.inputs)
        {
            if (!w.transforms.count(e))
                continue;
            if (!w.animations.count(e))
                continue;

            auto& transform = w.transforms[e];
            auto& anim      = w.animations[e];

            // Nếu đang chém thì cấm di chuyển (Tuỳ chọn: Nếu  muốn vừa đi vừa chém thì xóa dòng này)
            // if (w.combats.count(e) && w.combats[e].isAttacking)
            //{
            //    anim.moving = false;
            //    continue;  // Bỏ qua toàn bộ code di chuyển bên dưới
            //}

            ax::Vec2 moveVec(0, 0);

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
                moveVec.normalize();
                float currentSpeed = input.speed / 60.0f;
                ax::Vec2 velocity  = moveVec * currentSpeed;

                if (w.collisions.count(e))
                {
                    auto& col = w.collisions[e];
                    if (!col.isTrigger)
                    {
                        // Hàm Lambda kiểm tra va chạm với Entity khác
                        auto checkEntityCollision = [&](const ax::Rect& nextBox) -> bool {
                            for (auto& [otherEntity, otherCol] : w.collisions)
                            {
                                if (e == otherEntity)
                                    continue;  // Bỏ qua chính mình
                                if (otherCol.isTrigger)
                                    continue;  // Đi xuyên qua Item dưới đất

                                // Nếu Hitbox của mình đè lên Hitbox của đứa khác -> Bị chặn
                                if (nextBox.intersectsRect(otherCol.getWorldHitbox(w.transforms[otherEntity].position)))
                                    return true;
                            }
                            return false;
                        };

                        // Kiểm tra trục X (Vừa check Tường, Vừa check NPC)
                        ax::Vec2 nextPosX = transform.position + ax::Vec2(velocity.x, 0);
                        ax::Rect boxX     = col.getWorldHitbox(nextPosX);
                        if (!w.map.isCollision(boxX) && !checkEntityCollision(boxX))
                        {
                            transform.position.x = nextPosX.x;
                        }

                        // Kiểm tra trục Y (Vừa check Tường, Vừa check NPC)
                        ax::Vec2 nextPosY = transform.position + ax::Vec2(0, velocity.y);
                        ax::Rect boxY     = col.getWorldHitbox(nextPosY);
                        if (!w.map.isCollision(boxY) && !checkEntityCollision(boxY))
                        {
                            transform.position.y = nextPosY.y;
                        }
                    }
                    else
                    {
                        transform.position += velocity;  // Trigger (Item) thì trôi tự do
                    }
                }
                else
                {
                    transform.position += velocity;
                }

                anim.moving = true;
                if (std::abs(moveVec.x) > std::abs(moveVec.y))
                    anim.currentDir = (moveVec.x > 0) ? AnimationComponent::RIGHT : AnimationComponent::LEFT;
                else
                    anim.currentDir = (moveVec.y > 0) ? AnimationComponent::UP : AnimationComponent::DOWN;
            }
            else
            {
                anim.moving = false;
            }
        }
    }
} // namespace Systems
