#pragma once
#include "world/World.h"
#include "input/GameInput.h"
#include <string>

namespace Systems
{
    void UpdateInput(World& w, GameInput* gameInput);
    // MOVEMENT SYSTEM
    // Trong ecs/Systems.h -> namespace Systems
    void Movement(World& w);
    // RENDER SYSTEM
    void Render(World& w);
    std::string getPath(const std::string& base, int frame, AnimationComponent::Direction dir);
    // ANIMATION SYSTEM
    void Animation(World& w, float dt);
}  // namespace Systems
