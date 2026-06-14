#include "ecs/EntityFactory.h"
#include "axmol.h"
USING_NS_AX;

Entity EntityFactory::create(World& world, const EntityConfig& cfg, float x, float y)
{
    Entity e = world.createEntity();

    // 1. Transform + Sprite
    world.transforms[e] = {ax::Vec2(x, y)};
    auto sprite         = ax::Sprite::create(cfg.sprite + "_down_1.png");

    if (!sprite)
    {
        AXLOG("FAILED LOAD: %s", (cfg.sprite + "_down_1.png").c_str());
        return -1;
    }

    sprite->getTexture()->setAliasTexParameters();
    sprite->setScale(cfg.scale);
    world.worldNode->addChild(sprite, 1);
    world.sprites[e] = {sprite};

    // 2. Animation
    world.animations[e]           = {};
    world.animations[e].basePath  = cfg.sprite;
    world.animations[e].maxFrames = cfg.frames;

    // 3. Khởi tạo Hitbox
    CollisionComponent col;
    col.hitbox    = ax::Rect(0, 0, cfg.hitbox_w * cfg.scale, cfg.hitbox_h * cfg.scale);
    col.offset    = ax::Vec2(cfg.offset_x * cfg.scale, cfg.offset_y * cfg.scale);
    col.isTrigger = false;

    // Khởi tạo Chỉ số sinh tồn
    HealthComponent health;
    health.maxHp     = cfg.max_hp;
    health.hp        = cfg.max_hp;  // Sinh ra là đầy máu

    // Nạp dữ liệu Thể lực thực tế từ JSON
    health.maxStamina       = cfg.max_stamina;
    health.stamina          = cfg.max_stamina;
    health.staminaRegenRate = cfg.stamina_regen;

    health.maxMana   = cfg.max_mana;
    health.mana      = cfg.max_mana; // Sinh ra là đầy Mana
    health.manaRegenRate = cfg.mana_regen;// Lấy tốc độ hồi Mana từ JSON
    world.healths[e] = health;

    // Khởi tạo Hồ sơ chiến đấu (Hoàn toàn lấy từ file cấu hình)
    CombatComponent combat;
    combat.damage             = cfg.damage;
    combat.attackCooldown     = cfg.atk_cooldown;  // Đọc trực tiếp từ file cấu hình sạch sẽ
    combat.attackBoxLength    = cfg.atk_len;
    combat.attackBoxThickness = cfg.atk_thick;
    combat.attackOffset       = cfg.atk_offset;
    combat.spriteShift        = cfg.sprite_shift;
    world.combats[e]          = combat;

    // 4. Phân loại Component đặc thù theo loại thực thể
    if (cfg.type == "player")
    {
        world.inputs[e]       = {};
        world.inputs[e].speed = cfg.speed;
        world.playerEntity    = e;
        col.type              = CollisionType::Player;
    }
    else if (cfg.type == "npc")
    {
        col.type = CollisionType::NPC;

        world.inputs[e]       = {};
        world.inputs[e].speed = cfg.speed;
        AIComponent ai;
        ai.currentState = AIState::PATROL;
        ai.homePos      = ax::Vec2(x, y);
        world.ais[e]    = ai;
    }
    else if (cfg.type == "enemy")
    {
        col.type = CollisionType::Enemy;

        world.inputs[e]       = {};
        world.inputs[e].speed = cfg.speed;
        AIComponent ai;
        ai.currentState = AIState::IDLE;
        ai.homePos      = ax::Vec2(x, y);
        world.ais[e]    = ai;
    }
    else if (cfg.type == "item")
    {
        col.type      = CollisionType::Item;
        col.isTrigger = true;
    }

    world.collisions[e] = col;

    return e;
}
