#pragma once
#include "axmol.h"
#include "map/MapManager.h"  //Gọi đúng class Map của ông

class PathFinding
{
public:
    /**
     * @brief Tìm điểm tiếp theo cần bước tới bằng thuật toán A*
     */
    static ax::Vec2 findNextStep(MapManager& map, const ax::Vec2& startPos, const ax::Vec2& targetPos);
    //Kiểm tra tầm nhìn thẳng xem có bị vướng tường không
    static bool hasLineOfSight(MapManager& map, const ax::Vec2& startPos, const ax::Vec2& targetPos);
};
