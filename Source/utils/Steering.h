#pragma once
#include "axmol.h"
#include "world/World.h"

class Steering
{
public:
    // Tính Vector hướng thẳng tới mục tiêu
    static ax::Vec2 Seek(const ax::Vec2& currentPos, const ax::Vec2& targetPos);

    // Tính Vector đẩy lùi nếu đụng mặt đồng đội (Tản bầy)
    static ax::Vec2 Separation(Entity myEntity, const ax::Vec2& myPos, World& w, float radius = 25.0f);
};
