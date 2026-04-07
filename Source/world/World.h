#pragma once
#include "axmol.h"
#include <unordered_map>
#include "ecs/Entity.h"
#include "ecs/components.h"
#include "core/InputManager.h"
#include "camera/CameraController.h"
#include "map/MapManager.h"

USING_NS_AX;

class World
{
public:
    ax::Node* worldNode = nullptr;

    InputManager input;
    CameraController camera;
    MapManager map;

    Entity playerEntity = -1;

    // COMPONENT STORAGE
    std::unordered_map<Entity, TransformComponent> transforms;
    std::unordered_map<Entity, SpriteComponent> sprites;
    std::unordered_map<Entity, InputComponent> inputs;
    std::unordered_map<Entity, AnimationComponent> animations;

    void init(ax::Scene* scene);
    void update(float dt);

    Entity createEntity();

private:
    int nextEntity = 0;
};
