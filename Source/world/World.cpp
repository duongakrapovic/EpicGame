#include "world/World.h"
#include "ecs/systems.h"
#include "data/ConfigLoader.h"
#include "ecs/EntityFactory.h"

USING_NS_AX;

Entity World::createEntity()
{
    return nextEntity++;
}

void World::init(Scene* scene)
{
    input.setup(scene);

    auto fileUtils = FileUtils::getInstance();
    fileUtils->addSearchPath("maps/chunks");
    fileUtils->addSearchPath("maps/tilesets");

    auto fu = FileUtils::getInstance();
    AXLOG("TMX: %d", fu->isFileExist("maps/chunks/chunk0_0.tmx"));
    AXLOG("TSX: %d", fu->isFileExist("maps/tilesets/tileset.tsx"));
    AXLOG("PNG: %d", fu->isFileExist("maps/tilesets/tileset.png"));

    worldNode = ax::Node::create();
    scene->addChild(worldNode);

    auto configs = ConfigLoader::loadEntityConfig("configs/entity_config.json");

    // tạo player tại (100,100)
    playerEntity = EntityFactory::create(*this, configs["player"], 100.0f, 100.0f);

    // sync transform (quan trọng)
    transforms[playerEntity].position = Vec2(100, 100);

    // LOAD 1 MAP DUY NHẤT

    map.loadSingleChunk(worldNode);
}

void World::update(float dt)
{
    if (playerEntity != -1 && inputs.count(playerEntity))
    {
        auto& pInput = inputs[playerEntity];
        pInput.up    = input.up;
        pInput.down  = input.down;
        pInput.left  = input.left;
        pInput.right = input.right;
    }

    Systems::Movement(*this);
    Systems::Animation(*this, dt);
    Systems::Render(*this);

    if (playerEntity != -1 && sprites.count(playerEntity))
    {
        camera.follow(worldNode, sprites[playerEntity].sprite);
    }
}
