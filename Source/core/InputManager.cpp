#include "core/InputManager.h"

USING_NS_AX;

void InputManager::setup(Node* node)
{
    auto listener = EventListenerKeyboard::create();

    listener->onKeyPressed = [this](EventKeyboard::KeyCode keyCode, Event*) {
        if (keyCode == EventKeyboard::KeyCode::KEY_W)
            up = true;
        if (keyCode == EventKeyboard::KeyCode::KEY_S)
            down = true;
        if (keyCode == EventKeyboard::KeyCode::KEY_A)
            left = true;
        if (keyCode == EventKeyboard::KeyCode::KEY_D)
            right = true;
    };

    listener->onKeyReleased = [this](EventKeyboard::KeyCode keyCode, Event*) {
        if (keyCode == EventKeyboard::KeyCode::KEY_W)
            up = false;
        if (keyCode == EventKeyboard::KeyCode::KEY_S)
            down = false;
        if (keyCode == EventKeyboard::KeyCode::KEY_A)
            left = false;
        if (keyCode == EventKeyboard::KeyCode::KEY_D)
            right = false;
    };

    node->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, node);
}
