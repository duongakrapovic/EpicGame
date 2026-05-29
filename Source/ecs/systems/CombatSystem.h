#pragma once
#include "world/World.h"

namespace Systems
{
/**
 * @brief Xử lý cơ chế tấn công, tính cooldown đòn đánh và trừ máu đối thủ
 */
void Combat(World& w, float dt);
}  // namespace Systems
