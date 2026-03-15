#pragma once
#include "axmol.h"
#include <unordered_map>

class InputManager
{
public:
    static InputManager* getInstance();

    // ===== AXMOL CALLBACKS =====
    void onKeyPressed(ax::EventKeyboard::KeyCode key);
    void onKeyReleased(ax::EventKeyboard::KeyCode key);

    void onMouseDown(ax::EventMouse* event);
    void onMouseUp(ax::EventMouse* event);
    void onMouseMove(ax::EventMouse* event);
    void onMouseScroll(ax::EventMouse* event);

    // ===== GAME QUERY =====
    bool isKeyDown(ax::EventKeyboard::KeyCode key);
    bool isMouseDown(ax::EventMouse::MouseButton button);

    ax::Vec2 getMousePosition();
    float getMouseScrollY();

    void endFrame();  // reset scroll mỗi frame

private:
    InputManager() = default;

    std::unordered_map<int, bool> _keys;
    std::unordered_map<int, bool> _mouseButtons;

    ax::Vec2 _mousePos = ax::Vec2::ZERO;
    float _scrollY = 0;
};
