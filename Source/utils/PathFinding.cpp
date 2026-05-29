#include "PathFinding.h"
#include <queue>
#include <unordered_map>
#include <cmath>

// Cấu trúc vector tọa độ Ô lưới (Grid)
struct IntVec2
{
    int x, y;
    bool operator==(const IntVec2& other) const { return x == other.x && y == other.y; }
};

// Cấu trúc Nút cho A*
struct AStarNode
{
    IntVec2 pos;
    float f_score;  // Tổng chi phí (g + h)

    // Ưu tiên điểm có f_score thấp nhất
    bool operator>(const AStarNode& other) const { return f_score > other.f_score; }
};

// Hàm Băm (Hash) để dùng IntVec2 trong unordered_map
namespace std
{
template <>
struct hash<IntVec2>
{
    size_t operator()(const IntVec2& k) const { return ((size_t)k.x << 16) ^ (size_t)k.y; }
};
}  // namespace std

ax::Vec2 PathFinding::findNextStep(MapManager& map, const ax::Vec2& startPos, const ax::Vec2& targetPos)
{
    const int TILE_SIZE = 16;

    // Quy đổi từ Tọa độ Pixel sang Tọa độ Lưới (Grid)
    IntVec2 start  = {(int)(startPos.x / TILE_SIZE), (int)(startPos.y / TILE_SIZE)};
    IntVec2 target = {(int)(targetPos.x / TILE_SIZE), (int)(targetPos.y / TILE_SIZE)};

    // Nếu đã đứng chung 1 ô lưới thì đi thẳng tới Player luôn
    if (start == target)
        return targetPos;

    std::priority_queue<AStarNode, std::vector<AStarNode>, std::greater<AStarNode>> openSet;
    std::unordered_map<IntVec2, IntVec2> cameFrom;
    std::unordered_map<IntVec2, float> gScore;

    // Khoảng cách Manhattan (Heuristic)
    auto heuristic = [](IntVec2 a, IntVec2 b) { return std::abs(a.x - b.x) + std::abs(a.y - b.y); };

    openSet.push({start, (float)heuristic(start, target)});
    gScore[start] = 0.0f;

    // Giới hạn 200 vòng lặp (Chỉ tìm trong bán kính gần để tránh tụt FPS)
    int maxSteps        = 200;
    int steps           = 0;
    IntVec2 closestNode = start;
    float minH          = heuristic(start, target);

    while (!openSet.empty() && steps < maxSteps)
    {
        IntVec2 current = openSet.top().pos;
        openSet.pop();
        steps++;

        if (current == target)
        {
            closestNode = current;
            break;
        }

        // 4 hướng di chuyển: Lên, Xuống, Trái, Phải
        IntVec2 dirs[4] = {{0, 1}, {0, -1}, {-1, 0}, {1, 0}};

        for (auto& dir : dirs)
        {
            IntVec2 neighbor = {current.x + dir.x, current.y + dir.y};

            // Tái tạo lại tâm của ô lưới để check va chạm (Khung 28x28 bằng đúng con Orc)
            float cx = neighbor.x * TILE_SIZE + (TILE_SIZE / 2.0f);
            float cy = neighbor.y * TILE_SIZE + (TILE_SIZE / 2.0f);
            ax::Rect testBox(cx - 14, cy - 14, 28, 28);

            // Nếu ô đó là tường -> Bỏ qua
            if (map.isCollision(testBox))
                continue;

            float tentative_g = gScore[current] + 1.0f;  // Chi phí sang ô bên cạnh là 1

            if (gScore.find(neighbor) == gScore.end() || tentative_g < gScore[neighbor])
            {
                cameFrom[neighbor] = current;
                gScore[neighbor]   = tentative_g;

                float h = heuristic(neighbor, target);
                openSet.push({neighbor, tentative_g + h});

                // Lưu lại ô gần Player nhất trong trường hợp không tìm được đường tới tận nơi
                if (h < minH)
                {
                    minH        = h;
                    closestNode = neighbor;
                }
            }
        }
    }

    // Nếu không tìm thấy đường, trả về đứng im
    if (cameFrom.find(closestNode) == cameFrom.end())
        return startPos;

    // Truy vết ngược từ đích về lại điểm xuất phát để lấy BƯỚC ĐI KẾ TIẾP
    IntVec2 step = closestNode;
    while (cameFrom[step] != start)
    {
        // Kiểm tra an toàn tránh lặp vô tận
        if (cameFrom.find(cameFrom[step]) == cameFrom.end())
            break;
        step = cameFrom[step];
    }

    // Trả về tọa độ Pixel ở tâm của Ô lưới tiếp theo
    return ax::Vec2(step.x * TILE_SIZE + (TILE_SIZE / 2.0f), step.y * TILE_SIZE + (TILE_SIZE / 2.0f));
}

bool PathFinding::hasLineOfSight(MapManager& map, const ax::Vec2& startPos, const ax::Vec2& targetPos)
{
    ax::Vec2 dir = targetPos - startPos;
    float dist   = dir.length();

    // Nếu khoảng cách quá nhỏ (đứng đè lên nhau) thì coi như nhìn thấy luôn
    if (dist < 1.0f)
        return true;

    dir.normalize();
    float stepSize = 8.0f;  // Quét kiểm tra mỗi 8 pixel

    // Bắn một tia từ Quái tới Player
    for (float i = 0; i < dist; i += stepSize)
    {
        ax::Vec2 checkPos = startPos + dir * i;

        // Khung kiểm tra nhỏ gọn (16x16) để quét xem tia nhìn có sượt qua mép tường không
        ax::Rect testBox(checkPos.x - 8, checkPos.y - 8, 16, 16);

        // Nếu tia nhìn đập vào tường -> Bị khuất tầm nhìn
        if (map.isCollision(testBox))
        {
            return false;
        }
    }
    // Không chạm tường -> Nhìn thấy rõ ràng
    return true;
}
