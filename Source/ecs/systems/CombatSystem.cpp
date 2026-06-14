
#include "CombatSystem.h"
namespace Systems
{

    // =========================================================================
    // CÁC HÀM TRỢ THỦ (Chỉ dùng nội bộ trong file này)
    // =========================================================================

    // 1. Hàm lo việc Hồi phục và Đếm ngược thời gian bất tử
    static void UpdateRegenAndIFrames(World& w, float dt)
    {
        for (auto& [e, health] : w.healths)
        {
            if (health.invincibleTimer > 0)
                health.invincibleTimer -= dt;

            // Logic Hồi Mana (Đã comment lại nếu chưa dùng)
            if (health.mana < health.maxMana)
            {
                health.manaAccumulator += health.manaRegenRate * dt;
                if (health.manaAccumulator >= 1.0f)
                {
                    int regenAmount = (int)health.manaAccumulator;
                    health.mana += regenAmount;
                    health.manaAccumulator -= regenAmount;

                    if (health.mana > health.maxMana)
                        health.mana = health.maxMana;
                    if (e == w.playerEntity && w.hud)
                        w.hud->updateMana(health.mana, health.maxMana);
                }
            }

            // Logic Hồi Stamina
            if (health.stamina < health.maxStamina)
            {
                health.staminaAccumulator += health.staminaRegenRate * dt;
                if (health.staminaAccumulator >= 1.0f)
                {
                    int regenAmount = (int)health.staminaAccumulator;
                    health.stamina += regenAmount;
                    health.staminaAccumulator -= regenAmount;

                    if (health.stamina > health.maxStamina)
                        health.stamina = health.maxStamina;
                    if (e == w.playerEntity && w.hud)
                        w.hud->updateStamina(health.stamina, health.maxStamina);
                }
            }
        }
    }

    // 2. Hàm lo việc tính toán vị trí của Vết Chém (Hitbox)
    static ax::Rect CalculateAttackBox(const TransformComponent& transform,
                                       const AnimationComponent& anim,
                                       const CombatComponent& combat)
    {
        float length    = combat.attackBoxLength;
        float thickness = combat.attackBoxThickness;
        float offset    = combat.attackOffset;

        if (anim.currentDir == AnimationComponent::UP)
            return ax::Rect(transform.position.x - thickness / 2, transform.position.y + offset, thickness, length);
        else if (anim.currentDir == AnimationComponent::DOWN)
            return ax::Rect(transform.position.x - thickness / 2, transform.position.y - offset - length, thickness,
                            length);
        else if (anim.currentDir == AnimationComponent::LEFT)
            return ax::Rect(transform.position.x - offset - length, transform.position.y - thickness / 2, length,
                            thickness);
        else  // RIGHT
            return ax::Rect(transform.position.x + offset, transform.position.y - thickness / 2, length, thickness);
    }

    // 3. Hàm lo việc Kiểm tra va chạm và Trừ máu nạn nhân
    static void ProcessMeleeHits(World& w, Entity attackerEntity, const ax::Rect& attackBox, const CombatComponent& combat)
    {
        auto attackerType = w.collisions[attackerEntity].type;

        for (auto& [targetEntity, targetCol] : w.collisions)
        {
            if (targetEntity == attackerEntity)
                continue;  // Không tự chém mình

            bool isPlayerHittingEnemy = (attackerType == CollisionType::Player && targetCol.type == CollisionType::Enemy);
            bool isEnemyHittingPlayer = (attackerType == CollisionType::Enemy && targetCol.type == CollisionType::Player);

            if ((isPlayerHittingEnemy || isEnemyHittingPlayer) && w.healths.count(targetEntity))
            {
                ax::Rect targetBox = targetCol.getWorldHitbox(w.transforms[targetEntity].position);

                if (attackBox.intersectsRect(targetBox))
                {
                    auto& targetHealth = w.healths[targetEntity];

                    if (targetHealth.invincibleTimer <= 0)
                    {
                        targetHealth.hp -= combat.damage;
                        targetHealth.invincibleTimer = 0.5f;

                        if (targetEntity == w.playerEntity && w.hud)
                            w.hud->updateHP(targetHealth.hp, targetHealth.maxHp);

                        AXLOG("Chem trung! HP con: %d", targetHealth.hp);

                        if (targetHealth.hp <= 0 && !targetHealth.isDead)
                        {
                            targetHealth.isDead = true;
                            AXLOG("Da CHET!");
                            if (isPlayerHittingEnemy && w.hud)
                                w.hud->addKill(1);
                        }
                    }
                }
            }
        }
    }

    // =========================================================================
    // HÀM CHÍNH (Được gọi từ GameScene)
    // =========================================================================

    void Combat(World& w, float dt)
    {
        // Bước 1: Cập nhật các chỉ số sinh tồn (Hồi phục & I-Frames)
        UpdateRegenAndIFrames(w, dt);

        // Bước 2: Xử lý hành động tấn công
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
                    // Trừ Stamina
                    if (e == w.playerEntity && w.healths.count(e))
                    {
                        auto& health    = w.healths[e];
                        int staminaCost = 5;

                        if (health.stamina >= staminaCost)
                        {
                            health.stamina -= staminaCost;
                            if (e == w.playerEntity && w.hud)
                                w.hud->updateStamina(health.stamina, health.maxStamina);
                        }
                        else
                            continue;  // Hết sức -> Bỏ qua nhát chém này
                    }

                    combat.isAttacking   = true;
                    combat.cooldownTimer = combat.attackCooldown;
                    anim.frame           = 0;
                    anim.timer           = 0.0f;

                    // Tính toán Hitbox
                    combat.currentAttackBox = CalculateAttackBox(transform, anim, combat);

                    if (w.debugDraw)
                    {
                        w.debugDraw->drawSolidRect(combat.currentAttackBox.origin,
                                                   combat.currentAttackBox.origin + combat.currentAttackBox.size,
                                                   ax::Color4F(1, 1, 0, 0.5f));
                    }

                    // Xử lý đâm chém
                    ProcessMeleeHits(w, e, combat.currentAttackBox, combat);
                }
                else if (combat.cooldownTimer <= 0)
                {
                    combat.isAttacking = false;
                }
            }
        }
    }
}  // namespace Systems
