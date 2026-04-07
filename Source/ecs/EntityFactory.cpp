#include "ecs/EntityFactory.h"
#include "axmol.h"
USING_NS_AX;

Entity EntityFactory::create(World& world, const EntityConfig& cfg, float x, float y)
{
    Entity e = world.createEntity();

    // Transform
    world.transforms[e] = {ax::Vec2(x, y)};

    // 🔥 tạo sprite trước
    auto sprite = ax::Sprite::create(cfg.sprite);

    if (!sprite)
    {
        AXLOG("FAILED LOAD: %s", cfg.sprite.c_str());
        return -1;
    }

    sprite->setScale(cfg.scale);

    // 🔥 add vào world sau khi tạo
    world.worldNode->addChild(sprite, 1);

    world.sprites[e] = {sprite};

    // Animation
    world.animations[e]          = {};
    world.animations[e].basePath = cfg.sprite;

    // Player
    if (cfg.type == "player")
    {
        world.inputs[e]    = {};
        world.playerEntity = e;
    }

    return e;
}
