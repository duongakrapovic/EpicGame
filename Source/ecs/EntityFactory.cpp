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

    // --- ĐỒNG BỘ SCALE & KHỬ NHÒE ---
    sprite->getTexture()->setAliasTexParameters();  // Làm nét pixel
    sprite->setScale(1.0f);                         // Đưa về 1.0, việc to lên sẽ do Camera/WorldNode lo
    // -------------------------------

    world.worldNode->addChild(sprite, 1);
    world.sprites[e] = {sprite};

    // 2. Animation
    world.animations[e]          = {};
    world.animations[e].basePath = cfg.sprite;

    // 3. Tạo Hitbox (Giữ nguyên logic của bạn)
    CollisionComponent col;
    col.hitbox          = ax::Rect(0, 0, 12, 14);// x y width height
    //col.offset          = ax::Vec2(0, -5);
    col.type            = CollisionType::Player;
    col.isTrigger       = false;
    world.collisions[e] = col;

    // 4. Player logic
    if (cfg.type == "player")
    {
        world.inputs[e]    = {};
        world.inputs[e].speed = cfg.speed;
        world.playerEntity = e;
    }

    return e;
}
