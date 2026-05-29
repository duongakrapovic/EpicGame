#include "CombatSystem.h"

namespace Systems
{
void Combat(World& w, float dt)
{
    //Chạy đồng hồ I-Frames cho tất cả mọi người
    for (auto& [e, health] : w.healths)
    {
        if (health.invincibleTimer > 0)
            health.invincibleTimer -= dt;
    }

    for (auto& [e, combat] : w.combats)
    {
        if (combat.cooldownTimer > 0)
            combat.cooldownTimer -= dt;

        if (w.inputs.count(e) && w.transforms.count(e) && w.animations.count(e))
        {
            auto& input     = w.inputs[e];
            auto& transform = w.transforms[e];
            auto& anim      = w.animations[e];

            if (input.attack && combat.cooldownTimer <= 0)
            {
                combat.isAttacking   = true;
                combat.cooldownTimer = combat.attackCooldown;

                // Reset animation frame về 0 ngay khi bắt đầu chém
                anim.frame = 0;
                anim.timer = 0.0f;

                // Tính toán vùng sát thương (Attack Hitbox)
                ax::Rect attackBox;
                float length    = combat.attackBoxLength;
                float thickness = combat.attackBoxThickness;
                float offset    = combat.attackOffset;

                if (anim.currentDir == AnimationComponent::UP)
                    attackBox = ax::Rect(transform.position.x - thickness / 2, transform.position.y + offset, thickness,
                                         length);
                else if (anim.currentDir == AnimationComponent::DOWN)
                    attackBox = ax::Rect(transform.position.x - thickness / 2, transform.position.y - offset - length,
                                         thickness, length);
                else if (anim.currentDir == AnimationComponent::LEFT)
                    attackBox = ax::Rect(transform.position.x - offset - length, transform.position.y - thickness / 2,
                                         length, thickness);
                else if (anim.currentDir == AnimationComponent::RIGHT)
                    attackBox = ax::Rect(transform.position.x + offset, transform.position.y - thickness / 2, length,
                                         thickness);

                combat.currentAttackBox = attackBox;

                if (w.debugDraw)
                {
                    w.debugDraw->drawSolidRect(attackBox.origin, attackBox.origin + attackBox.size,
                                               ax::Color4F(1, 1, 0, 0.5f));
                }

                // Xác định "Phe" của kẻ đang vung vũ khí
                auto attackerType = w.collisions[e].type;

                for (auto& [targetEntity, targetCol] : w.collisions)
                {
                    if (targetEntity == e)
                        continue;  // Không tự chém chính mình

                    // Luật Phân Phe: Ai được quyền chém ai?
                    bool isPlayerHittingEnemy =
                        (attackerType == CollisionType::Player && targetCol.type == CollisionType::Enemy);
                    bool isEnemyHittingPlayer =
                        (attackerType == CollisionType::Enemy && targetCol.type == CollisionType::Player);

                    // Chỉ xử lý sát thương nếu đúng phe VÀ nạn nhân có máu
                    if ((isPlayerHittingEnemy || isEnemyHittingPlayer) && w.healths.count(targetEntity))
                    {
                        ax::Rect targetBox = targetCol.getWorldHitbox(w.transforms[targetEntity].position);
                        if (attackBox.intersectsRect(targetBox))
                        {
                            auto& targetHealth = w.healths[targetEntity];

                            // [ĐÃ CẬP NHẬT I-FRAMES Ở BƯỚC TRƯỚC]
                            if (targetHealth.invincibleTimer <= 0)
                            {
                                targetHealth.hp -= combat.damage;
                                targetHealth.invincibleTimer = 0.5f;  // Chớp đỏ 0.5s bất tử

                                AXLOG("Chem trung! HP con: %d", targetHealth.hp);

                                if (targetHealth.hp <= 0 && !targetHealth.isDead)
                                {
                                    targetHealth.isDead = true;
                                    AXLOG("Da CHET!");
                                }
                            }
                        }
                    }
                }
            }
            else if (combat.cooldownTimer <= 0)
            {
                combat.isAttacking = false;
            }
        }
    }
}
}  // namespace Systems
