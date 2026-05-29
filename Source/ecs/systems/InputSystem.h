#pragma once
#include "world/World.h"
#include "input/GameInput.h"  // Cần thiết để đọc trạng thái phím từ lớp GameInput

namespace Systems
{
/**
 * @brief Cập nhật trạng thái phím bấm của người chơi vào InputComponent
 */
void UpdateInput(World& w, GameInput* gameInput);
}  // namespace Systems
