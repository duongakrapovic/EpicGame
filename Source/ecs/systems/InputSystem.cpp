#include "InputSystem.h"
namespace Systems
{
    void UpdateInput(World& w, GameInput* gameInput)
    {
        if (!gameInput)
            return;

        if (w.inputs.count(w.playerEntity))
        {
            auto& inputComp = w.inputs[w.playerEntity];

            inputComp.up    = gameInput->isKeyPressed(ax::EventKeyboard::KeyCode::KEY_W);
            inputComp.down  = gameInput->isKeyPressed(ax::EventKeyboard::KeyCode::KEY_S);
            inputComp.left  = gameInput->isKeyPressed(ax::EventKeyboard::KeyCode::KEY_A);
            inputComp.right = gameInput->isKeyPressed(ax::EventKeyboard::KeyCode::KEY_D);

            inputComp.attack   = gameInput->isKeyPressed(ax::EventKeyboard::KeyCode::KEY_J);
            inputComp.interact = gameInput->isKeyPressed(ax::EventKeyboard::KeyCode::KEY_F);
        }
    }
}// namespace Systems
