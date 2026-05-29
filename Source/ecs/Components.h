#pragma once
#include "axmol.h"
#include <string>

struct TransformComponent
{
    ax::Vec2 position;
};

struct SpriteComponent
{
    ax::Sprite* sprite = nullptr;
};

struct InputComponent
{
    bool up       = false;
    bool down     = false;
    bool left     = false;
    bool right    = false;
    bool attack   = false;
    bool interact = false;
    float speed   = 0.0f;
};

enum class AnimState
{
    IDLE,
    WALK,
    ATTACK
};

struct AnimationComponent
{
    enum Direction
    {
        DOWN,
        UP,
        LEFT,
        RIGHT
    };
    Direction currentDir   = DOWN;
    AnimState currentState = AnimState::IDLE;
    float timer            = 0;
    int frame              = 0;
    int maxFrames          = 1;
    bool moving            = false;
    bool hasWeapon         = false;
    std::string basePath;
};

enum class CollisionType
{
    None = 0,
    Player,
    NPC,
    Enemy,
    Item,
    Wall
};

struct CollisionComponent
{
    ax::Rect hitbox;
    ax::Vec2 offset;
    CollisionType type = CollisionType::None;
    bool isTrigger     = false;

    ax::Rect getWorldHitbox(const ax::Vec2& entityPos) const
    {
        return ax::Rect(entityPos.x + offset.x - hitbox.size.width / 2, entityPos.y + offset.y - hitbox.size.height / 2,
                        hitbox.size.width, hitbox.size.height);
    }
};

struct HealthComponent
{
    int hp                = 100;
    int maxHp             = 100;
    int mana              = 50;
    int maxMana           = 50;
    bool isDead           = false;
    float invincibleTimer = 0.0f;  // Đồng hồ đếm ngược I-Frames khi trúng đòn
};

struct CombatComponent
{
    int damage           = 10;
    float attackCooldown = 0.5f;  // Thời gian giãn cách giữa các đòn đánh
    float cooldownTimer  = 0.0f;
    bool isAttacking     = false;
    ax::Rect currentAttackBox;
    float attackBoxLength    = 30.0f;
    float attackBoxThickness = 10.0f;
    float attackOffset       = 15.0f;
    float spriteShift        = 0.0f;
};

enum class AIState
{
    IDLE,
    PATROL,
    CHASE,
    ATTACK
};

struct AIComponent
{
    AIState currentState = AIState::IDLE;
    float timer          = 0.0f;
    ax::Vec2 targetPos   = ax::Vec2::ZERO;
    ax::Vec2 homePos     = ax::Vec2::ZERO;
};
