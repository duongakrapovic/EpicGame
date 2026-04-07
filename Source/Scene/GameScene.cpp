#include "scene/GameScene.h"

USING_NS_AX;

bool GameScene::init()
{
    if (!Scene::init())
        return false;
    world.init(this);

    scheduleUpdate();

    return true;
}
void GameScene::update(float dt)
{
    world.update(dt);
}
