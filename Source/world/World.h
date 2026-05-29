#pragma once
#include "axmol.h"
#include <unordered_map>
#include "ecs/Entity.h"
#include "ecs/Components.h"
#include "camera/CameraController.h"
#include "map/MapManager.h"

class GameInput;  // Forward declaration

USING_NS_AX;

class World
{
public:
    ax::Node* worldNode     = nullptr;
    ax::DrawNode* debugDraw = nullptr;

    CameraController camera;
    MapManager map;

    Entity playerEntity = -1;

    // COMPONENT STORAGE
    std::unordered_map<Entity, TransformComponent> transforms;
    std::unordered_map<Entity, SpriteComponent> sprites;
    std::unordered_map<Entity, InputComponent> inputs;
    std::unordered_map<Entity, AnimationComponent> animations;
    std::unordered_map<Entity, CollisionComponent> collisions;

    // CHỈ SỐ SINH TỒN VÀ CHIẾN ĐẤU
    std::unordered_map<Entity, HealthComponent> healths;
    std::unordered_map<Entity, CombatComponent> combats;

    // AI
    std::unordered_map<Entity, AIComponent> ais;

    void init(ax::Scene* scene);
    void update(float dt, GameInput* input);  // Đã sửa để nhận con trỏ Input

    Entity createEntity();

private:
    int nextEntity = 0;
};
