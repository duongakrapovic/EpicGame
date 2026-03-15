#include "InputManager.h"

static InputManager* s_instance = nullptr;

InputManager* InputManager::getInstance()
{
    if (!s_instance)
        s_instance = new InputManager();
    return s_instance;
}

// ================= KEYBOARD =================

void InputManager::onKeyPressed(ax::EventKeyboard::KeyCode key)
{
    _keys[(int)key] = true;
}

void InputManager::onKeyReleased(ax::EventKeyboard::KeyCode key)
{
    _keys[(int)key] = false;
}

bool InputManager::isKeyDown(ax::EventKeyboard::KeyCode key)
{
    return _keys[(int)key];
}

// ================= MOUSE =================

void InputManager::onMouseDown(ax::EventMouse* event)
{
    _mouseButtons[(int)event->getMouseButton()] = true;
}

void InputManager::onMouseUp(ax::EventMouse* event)
{
    _mouseButtons[(int)event->getMouseButton()] = false;
}

void InputManager::onMouseMove(ax::EventMouse* event)
{
    _mousePos = event->getLocationInView();
}

void InputManager::onMouseScroll(ax::EventMouse* event)
{
    _scrollY += event->getScrollY();
}

bool InputManager::isMouseDown(ax::EventMouse::MouseButton button)
{
    return _mouseButtons[(int)button];
}

ax::Vec2 InputManager::getMousePosition()
{
    return _mousePos;
}

float InputManager::getMouseScrollY()
{
    return _scrollY;
}

void InputManager::endFrame()
{
    _scrollY = 0;
}
