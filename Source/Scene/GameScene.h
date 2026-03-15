#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "axmol.h"

class GameScene : public ax::Scene
{
public:
    bool init() override;
    void update(float delta) override;

private:
    ax::Layer* _worldLayer = nullptr;
    ax::Sprite* _player    = nullptr;
    ax::Sprite* _map       = nullptr;
};

#endif
