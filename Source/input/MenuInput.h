#pragma once
#include "axmol.h"

class MenuInput
{
public:
    // Ta truyền vào một Node (có thể là StartScene hoặc SettingsLayer)
    MenuInput(ax::Node* target);
    ~MenuInput();

private:
    void onKeyPressed(ax::EventKeyboard::KeyCode keyCode, ax::Event* event);
    ax::Node* _target;
    ax::EventListenerKeyboard* _listener;
};
