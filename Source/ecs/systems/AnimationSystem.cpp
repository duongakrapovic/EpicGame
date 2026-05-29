#include "AnimationSystem.h"
#include <string>

namespace Systems
{
    // Hàm phụ trợ nối chuỗi tạo đường dẫn ảnh (Chỉ dùng nội bộ trong file này)
    std::string getPath(const std::string& basePath, int frame, AnimationComponent::Direction dir, AnimState state)
    {
        std::string dirStr;
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

        std::string actionStr = "";
        if (state == AnimState::ATTACK)
        {
            if (basePath.find("boy") != std::string::npos)
                actionStr = "_axe";
            else
                actionStr = "_attack";
        }

        return basePath + actionStr + "_" + dirStr + "_" + std::to_string(frame + 1) + ".png";
    }

    void Animation(World& w, float dt)
    {
        for (auto& [e, anim] : w.animations)
        {
            if (!w.sprites.count(e))
                continue;
            auto& sprite = w.sprites[e];

            if (w.combats.count(e))
            {
                if (w.combats[e].isAttacking)
                    anim.currentState = AnimState::ATTACK;
                else
                    anim.currentState = (anim.moving) ? AnimState::WALK : AnimState::IDLE;
            }
            else
            {
                anim.currentState = (anim.moving) ? AnimState::WALK : AnimState::IDLE;
            }

            if (anim.maxFrames > 1 && (anim.currentState == AnimState::WALK || anim.currentState == AnimState::ATTACK))
            {
                anim.timer += dt;
                float frameDuration = (anim.currentState == AnimState::ATTACK) ? 0.15f : 0.2f;

                if (anim.timer > frameDuration)
                {
                    anim.timer = 0;
                    anim.frame++;
                    if (anim.frame >= anim.maxFrames)
                    {
                        anim.frame = 0;
                        if (anim.currentState == AnimState::ATTACK && w.combats.count(e))
                        {
                            w.combats[e].isAttacking = false;
                        }
                    }
                }
            }
            else if (anim.currentState == AnimState::IDLE)
            {
                anim.frame = 0;
            }

            std::string newPath = getPath(anim.basePath, anim.frame, anim.currentDir, anim.currentState);
            auto texture        = ax::Director::getInstance()->getTextureCache()->addImage(newPath);

            if (texture && sprite.sprite->getTexture() != texture)
            {
                texture->setAliasTexParameters();

                //Dùng SpriteFrame thay vì setTexture thông thường
                // Cách này ép Engine phải xóa khung cũ và dựng lại khung mới khớp 100% tỷ lệ ảnh
                auto spriteFrame = ax::SpriteFrame::createWithTexture(
                    texture, ax::Rect(0, 0, texture->getContentSize().width, texture->getContentSize().height));
                sprite.sprite->setSpriteFrame(spriteFrame);
            }
        }
    }
}  // namespace Systems
