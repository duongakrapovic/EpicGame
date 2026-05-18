#include "ecs/systems.h"
#include <cmath>

namespace Systems
{
    void UpdateInput(World& w, GameInput* gameInput)
    {
        if (!gameInput)
            return;

        // Chỉ cập nhật cho Player Entity
        if (w.inputs.count(w.playerEntity))
        {
            auto& inputComp = w.inputs[w.playerEntity];

            // Đọc trạng thái phím từ GameInput và đổ vào Component
            //TODO InputManager::getMoveUpKey()
            inputComp.up    = gameInput->isKeyPressed(ax::EventKeyboard::KeyCode::KEY_W);
            inputComp.down  = gameInput->isKeyPressed(ax::EventKeyboard::KeyCode::KEY_S);
            inputComp.left  = gameInput->isKeyPressed(ax::EventKeyboard::KeyCode::KEY_A);
            inputComp.right = gameInput->isKeyPressed(ax::EventKeyboard::KeyCode::KEY_D);
        }
    }

    void Movement(World& w)
    {
        for (auto& [e, input] : w.inputs)
        {
            //check component tồn tại
            if (!w.transforms.count(e))
                continue;

            if (!w.animations.count(e))
                continue;

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

    void Render(World& w)
    {
        if (!w.debugDraw)
        {
            return;
        }
        w.debugDraw->clear();

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
                w.debugDraw->drawRect(hb.origin, hb.origin + hb.size, ax::Color4F::RED);
            }
        }
    }

    std::string getPath(const std::string& base, int frame, AnimationComponent::Direction dir)
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

    void Animation(World& w, float dt)
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
                }
            }
            else
            {
                if (anim.frame != 0)
                {
                    anim.frame = 0;
                }
            }

            // Luôn update texture mỗi frame.
            // Tốn hơn một chút nhưng phản hồi đổi hướng là tức thì, không delay.
            std::string newPath = getPath(anim.basePath, anim.frame, anim.currentDir);

            auto texture = ax::Director::getInstance()->getTextureCache()->addImage(newPath);

            if (texture && sprite.sprite->getTexture() != texture)
            {
                texture->setAliasTexParameters();
                sprite.sprite->setTexture(texture);
            }
        }
    }
    }  // namespace Systems
