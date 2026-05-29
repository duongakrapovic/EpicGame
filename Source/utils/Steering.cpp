#include "Steering.h"

ax::Vec2 Steering::Seek(const ax::Vec2& currentPos, const ax::Vec2& targetPos)
{
    if (currentPos.distance(targetPos) < 1.0f)
        return ax::Vec2::ZERO;
    return (targetPos - currentPos).getNormalized();
}

ax::Vec2 Steering::Separation(Entity myEntity, const ax::Vec2& myPos, World& w, float radius)
{
    ax::Vec2 push(0, 0);
    for (auto& [otherE, otherTf] : w.transforms)
    {
        if (myEntity == otherE)
            continue;

        // Chỉ né đồng đội (Enemy né Enemy)
        if (w.collisions.count(otherE) && w.collisions[otherE].type == w.collisions[myEntity].type)
        {
            float dist = myPos.distance(otherTf.position);
            if (dist > 0 && dist < radius)
            {
                // Càng gần đẩy càng mạnh
                push += (myPos - otherTf.position).getNormalized() * (radius - dist);
            }
        }
    }
    return push;
}
