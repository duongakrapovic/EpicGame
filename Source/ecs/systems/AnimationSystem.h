#pragma once
#include "world/World.h"

namespace Systems
{
/**
 * @brief Cập nhật các trạng thái hoạt ảnh (Đi bộ, Đứng im, Tấn công) theo thời gian thực
 */
void Animation(World& w, float dt);
}  // namespace Systems
