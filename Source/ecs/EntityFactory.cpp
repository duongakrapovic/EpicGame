#include "ecs/EntityFactory.h"
#include "axmol.h"
USING_NS_AX;

Entity EntityFactory::create(World& world, const EntityConfig& cfg, float x, float y)
{
    Entity e = world.createEntity();

    // 1. Transform + Sprite
    world.transforms[e] = {ax::Vec2(x, y)};
    auto sprite         = ax::Sprite::create(cfg.sprite);

    if (!sprite)
    {
        AXLOG("FAILED LOAD: %s", cfg.sprite.c_str());
        return -1;
    }

    sprite->getTexture()->setAliasTexParameters();
    sprite->setScale(1.0f);
    world.worldNode->addChild(sprite, 1);
    world.sprites[e] = {sprite};

    // 2. Animation
    world.animations[e]          = {};
    world.animations[e].basePath = cfg.sprite;

    // =======================================================
    // 3. KHỞI TẠO HITBOX CHUNG (Lấy thông số từ JSON)
    // =======================================================
    CollisionComponent col;
    col.hitbox    = ax::Rect(0, 0, cfg.hitbox_w, cfg.hitbox_h);
    col.offset    = ax::Vec2(cfg.offset_x, cfg.offset_y);
    col.isTrigger = false;

    // 4. PHÂN LOẠI COMPONENT ĐẶC THÙ THEO TYPE
    if (cfg.type == "player")
    {
        world.inputs[e]       = {};
        world.inputs[e].speed = cfg.speed;
        world.playerEntity    = e;

        col.type = CollisionType::Player;
    }
    else if (cfg.type == "npc")
    {
        col.type = CollisionType::NPC;
    }
    else if (cfg.type == "enemy")
    {
        col.type = CollisionType::Enemy;
    }
    else if (cfg.type == "item")
    {
        col.type      = CollisionType::Item;
        col.isTrigger = true;  // Vật phẩm thì có thể đi xuyên qua để nhặt
    }

    // Cuối cùng, gán cái Hitbox đã được cấu hình xong vào World
    world.collisions[e] = col;

    return e;
}
