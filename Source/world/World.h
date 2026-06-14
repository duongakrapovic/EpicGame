#pragma once
#include "axmol.h"
#include <unordered_map>
#include "ecs/Entity.h"
#include "ecs/Components.h"
#include "camera/CameraController.h"
#include "map/MapManager.h"
#include "ui/HUD.h"
#include "data/ConfigLoader.h"

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

    // Lưu lại bộ config để dùng mọi lúc mọi nơi
    std::unordered_map<std::string, EntityConfig> _entityConfigs;

    // Đồng hồ đếm thời gian đẻ quái
    float _spawnTimer = 0.0f;

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

    HUD* hud = nullptr;

private:
    int nextEntity = 0;
};
