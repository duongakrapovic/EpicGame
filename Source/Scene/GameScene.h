#pragma once
#include "axmol.h"
#include "world/World.h"

class GameScene : public ax::Scene
{
public:
    virtual bool init();

    void update(float dt) override;
    CREATE_FUNC(GameScene);

private:
    World world;
};
