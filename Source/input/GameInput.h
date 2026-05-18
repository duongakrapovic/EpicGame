#pragma once
#include "axmol.h"
#include <map>
#include <functional>

class GameInput
{
public:
    GameInput(ax::Node* scene);
    ~GameInput();

    // ECS Systems sẽ gọi hàm này để check phím
    bool isKeyPressed(ax::EventKeyboard::KeyCode code);
    ax::Vec2 getMousePosition() const { return _mousePos; }

    std::function<void()> onEscapePressed = nullptr;

private:
    std::map<ax::EventKeyboard::KeyCode, bool> _keyStates;
    ax::Vec2 _mousePos;
    ax::Node* _scene;
};
