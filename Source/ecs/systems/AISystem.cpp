#include "AISystem.h"
#include "utils/PathFinding.h"
#include "utils/Steering.h"
#include <cmath>
#include <cstdlib>  // Cho hàm rand()

namespace Systems
{
    void AI(World& w, float dt)
    {
        // Kiểm tra xem Player còn sống (chưa bị hệ thống dọn rác xóa) không?
        bool isPlayerAlive = w.transforms.count(w.playerEntity) > 0;

        for (auto& [e, ai] : w.ais)
        {
            //Nếu Player chết, tắt chế độ tấn công, cho quái đứng im
            if (!isPlayerAlive)
            {
                ai.currentState = AIState::IDLE;
                if (w.inputs.count(e))
                {
                    w.inputs[e].attack = false;
                    w.inputs[e].up = w.inputs[e].down = w.inputs[e].left = w.inputs[e].right = false;
                }
                continue;  // Bỏ qua toàn bộ logic đuổi đánh bên dưới
            }
        }

        if (w.playerEntity == -1 || !w.transforms.count(w.playerEntity))
            return;
        ax::Vec2 playerPos = w.transforms[w.playerEntity].position;

        // Chỉ lặp qua những Entity có lắp Não bộ (AIComponent)
        for (auto& [e, ai] : w.ais)
        {
            if (!w.inputs.count(e) || !w.transforms.count(e))
                continue;

            auto& input     = w.inputs[e];
            auto& transform = w.transforms[e];
            auto& col       = w.collisions[e];
            ax::Vec2 myPos  = transform.position;

            input.up = input.down = input.left = input.right = input.attack = false;
            ai.timer -= dt;
            float distToPlayer = myPos.distance(playerPos);

            //TRẠNG THÁI HỮU HẠN
            switch (ai.currentState)
            {
            case AIState::IDLE:  // Trạng thái nghỉ: đi dạo
            {
                if (ai.timer <= 0)
                {
                    ai.timer        = 1.0f + (rand() % 3);
                    ai.currentState = AIState::PATROL;
                    // Chọn bừa 1 hướng quanh nhà
                    ai.targetPos    = myPos + ax::Vec2((rand() % 100) - 50, (rand() % 100) - 50);
                }

                // Nếu là Quái, thấy Player thì gầm lên đuổi theo
                if (col.type == CollisionType::Enemy && distToPlayer < 350.0f)
                {
                    ai.currentState = AIState::CHASE;
                    ai.timer        = 0;// Kích hoạt tính A* lập tức
                }
                break;
            }

            case AIState::PATROL:
            {
                // Đi dạo: Lấy vector đi tới điểm target
                ax::Vec2 moveDir = Steering::Seek(myPos, ai.targetPos);

                // Đi đến nơi hoặc hết giờ thì đứng nghỉ
                if (myPos.distance(ai.targetPos) < 5.0f || ai.timer <= 0)
                {
                    ai.timer        = 1.0f + (rand() % 2);
                    ai.currentState = AIState::IDLE;
                }

                // Nếu là Quái, đang dạo mà thấy Player thì đuổi theo
                if (col.type == CollisionType::Enemy && distToPlayer < 350.0f)
                {
                    ai.currentState = AIState::CHASE;
                    ai.timer        = 0;
                }

                // Bấm nút đi dạo
                if (moveDir.x > 0.3f)
                    input.right = true;
                else if (moveDir.x < -0.3f)
                    input.left = true;
                if (moveDir.y > 0.3f)
                    input.up = true;
                else if (moveDir.y < -0.3f)
                    input.down = true;
                break;
            }

            case AIState::CHASE:
            {
                // Truy đuổi: Mất dấu Player (đi quá xa) -> Về đứng im
                // Mất dấu Player (đi quá xa) -> Về đứng im
                if (distToPlayer > 200.0f)
                {
                    ai.currentState = AIState::IDLE;
                    break;
                }

                // Tính khoảng cách trục X và Y riêng biệt
                float dx = std::abs(playerPos.x - myPos.x);
                float dy = std::abs(playerPos.y - myPos.y);

                // Áp sát đủ gần và đứng thẳng hàng ngang HOẶC thẳng hàng dọc mới được chém
                if (distToPlayer <= 35.0f && (dx < 15.0f || dy < 15.0f))
                {
                    ai.currentState = AIState::ATTACK;

                     // Khóa mục tiêu để tung đòn chính xác
                    if (dx > dy)
                    {
                        input.right = (playerPos.x > myPos.x);
                        input.left  = !input.right;
                    }
                    else
                    {
                        input.up   = (playerPos.y > myPos.y);
                        input.down = !input.up;
                    }
                    break;
                }

                // Cứ 0.4s cập nhật đường đi (Line of sight hoặc A*)
                if (myPos.distance(ai.targetPos) < 5.0f)
                    ai.timer = 0;
                if (ai.timer <= 0)
                {
                    ai.timer = 0.4f;
                    if (PathFinding::hasLineOfSight(w.map, myPos, playerPos))
                    {
                        ai.targetPos = playerPos;
                    }
                    else
                    {
                        ai.targetPos = PathFinding::findNextStep(w.map, myPos, playerPos);
                    }
                }

                // Trộn lực: Lực hút về Target + Lực đẩy né Đồng đội
                ax::Vec2 seekForce       = Steering::Seek(myPos, ai.targetPos);
                ax::Vec2 separationForce = Steering::Separation(e, myPos, w);

                ax::Vec2 finalMoveDir = seekForce;
                if (separationForce.lengthSquared() > 0)
                {
                    finalMoveDir = (seekForce + separationForce.getNormalized() * 0.8f).getNormalized();
                }

                // Bấm nút chạy
                if (finalMoveDir.x > 0.3f)
                    input.right = true;
                else if (finalMoveDir.x < -0.3f)
                    input.left = true;
                if (finalMoveDir.y > 0.3f)
                    input.up = true;
                else if (finalMoveDir.y < -0.3f)
                    input.down = true;
                break;
            }

            case AIState::ATTACK:
            {
                // Tấn công: Bấm nút chém
                input.attack = true;

                // Nếu Player chạy lùi ra xa thì đuổi tiếp
                if (distToPlayer > 45.0f)
                {
                    ai.currentState = AIState::CHASE;
                    ai.timer        = 0;
                }
                break;
            }
            }
        }
    }
}  // namespace Systems
