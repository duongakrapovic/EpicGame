#include "RenderSystem.h"
#include <cmath>

namespace Systems
{
void Render(World& w)
{
    if (!w.debugDraw)
        return;
    w.debugDraw->clear();

    for (auto& [e, transform] : w.transforms)
    {
        // Tọa độ gốc (Dùng cho Hitbox - Tuyệt đối không thay đổi)
        float rx = std::round(transform.position.x);
        float ry = std::round(transform.position.y);

        // Tọa độ ảo (Dùng cho Sprite ảnh)
        float drawX = rx;
        float drawY = ry;

        // Nếu đang tấn công, đẩy ảo bức ảnh lên phía trước để bù trừ
        if (w.combats.count(e) && w.combats[e].isAttacking && w.animations.count(e))
        {
            float shift = w.combats[e].spriteShift;
            auto dir    = w.animations[e].currentDir;

            if (dir == AnimationComponent::RIGHT)
                drawX += shift;
            else if (dir == AnimationComponent::LEFT)
                drawX -= shift;
            else if (dir == AnimationComponent::UP)
                drawY += shift;
            else if (dir == AnimationComponent::DOWN)
                drawY -= shift;
        }

        // 1. Áp dụng tọa độ ẢO cho bức ảnh
        if (w.sprites.count(e))
        {
            w.sprites[e].sprite->setPosition(ax::Vec2(drawX, drawY));

            // [MỚI] Hiệu ứng chớp nháy khi trúng đòn
            if (w.healths.count(e))
            {
                auto& health = w.healths[e];
                if (health.invincibleTimer > 0)
                {
                    // Nếu đang bất tử: Đổi màu thành Đỏ và làm hơi mờ đi (Tạo hiệu ứng chớp)
                    w.sprites[e].sprite->setColor(ax::Color3B::RED);
                    // Dùng hàm sin để làm chớp nháy opacity
                    uint8_t opacity = (uint8_t)(155 + 100 * std::sin(health.invincibleTimer * 30.0f));
                    w.sprites[e].sprite->setOpacity(opacity);
                }
                else
                {
                    // Hết bất tử: Trả lại màu Trắng gốc và độ nét 100%
                    w.sprites[e].sprite->setColor(ax::Color3B::WHITE);
                    w.sprites[e].sprite->setOpacity(255);
                }
            }
        }

        // 2. Áp dụng tọa độ GỐC cho Hitbox Cơ thể
        if (w.collisions.count(e))
        {
            auto& col   = w.collisions[e];
            ax::Rect hb = col.getWorldHitbox(ax::Vec2(rx, ry));
            w.debugDraw->drawRect(hb.origin, hb.origin + hb.size, ax::Color4F::RED);
        }

        // 3. Khung chém (Màu Vàng) vẫn sinh ra từ gốc
        if (w.combats.count(e) && w.combats[e].isAttacking)
        {
            auto& combat = w.combats[e];
            w.debugDraw->drawSolidRect(combat.currentAttackBox.origin,
                                       combat.currentAttackBox.origin + combat.currentAttackBox.size,
                                       ax::Color4F(1, 1, 0, 0.5f));
        }
    }
}
}  // namespace Systems
