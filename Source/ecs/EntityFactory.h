#pragma once
#include "world/World.h"
#include "data/ConfigLoader.h"

class EntityFactory
{
public:
    static Entity create(World& world, const EntityConfig& cfg, float x, float y);
};
