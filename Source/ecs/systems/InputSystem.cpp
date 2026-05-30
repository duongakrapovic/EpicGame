#include "InputSystem.h"
#include "axmol.h"  // Bắt buộc phải include thư viện gốc của Axmol
//Thêm thư viện UserDefault để đọc file Save
#include "base/UserDefault.h"

namespace Systems
{
void UpdateInput(World& w, GameInput* gameInput)
{
    if (!gameInput)
        return;

    if (w.inputs.count(w.playerEntity))
    {
        auto& inputComp = w.inputs[w.playerEntity];
        auto ud         = ax::UserDefault::getInstance();

        // 1. Lấy các phím mà người chơi đã cài đặt từ UserDefault ra.
        // Nếu người chơi chưa bao giờ vào cài đặt, nó sẽ lấy các phím mặc định (W,S,A,D,J,F)
        auto currentKeyUp =
            (ax::EventKeyboard::KeyCode)ud->getIntegerForKey("Key_Up", (int)ax::EventKeyboard::KeyCode::KEY_W);
        auto currentKeyDown =
            (ax::EventKeyboard::KeyCode)ud->getIntegerForKey("Key_Down", (int)ax::EventKeyboard::KeyCode::KEY_S);
        auto currentKeyLeft =
            (ax::EventKeyboard::KeyCode)ud->getIntegerForKey("Key_Left", (int)ax::EventKeyboard::KeyCode::KEY_A);
        auto currentKeyRight =
            (ax::EventKeyboard::KeyCode)ud->getIntegerForKey("Key_Right", (int)ax::EventKeyboard::KeyCode::KEY_D);

        auto currentKeyAtk =
            (ax::EventKeyboard::KeyCode)ud->getIntegerForKey("Key_Attack", (int)ax::EventKeyboard::KeyCode::KEY_J);
        auto currentKeyInteract =
            (ax::EventKeyboard::KeyCode)ud->getIntegerForKey("Key_Interact", (int)ax::EventKeyboard::KeyCode::KEY_F);

        // 2. Cập nhật trạng thái input dựa trên các phím động vừa lấy được
        inputComp.up    = gameInput->isKeyPressed(currentKeyUp);
        inputComp.down  = gameInput->isKeyPressed(currentKeyDown);
        inputComp.left  = gameInput->isKeyPressed(currentKeyLeft);
        inputComp.right = gameInput->isKeyPressed(currentKeyRight);

        inputComp.attack   = gameInput->isKeyPressed(currentKeyAtk);
        inputComp.interact = gameInput->isKeyPressed(currentKeyInteract);
    }
}
}  // namespace Systems
