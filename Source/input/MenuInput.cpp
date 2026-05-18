#include "input/MenuInput.h"
#include "scene/StartScene.h"
#include "scene/settings/SettingsLayer.h"

USING_NS_AX;

MenuInput::MenuInput(Node* target) : _target(target)
{
    _listener               = EventListenerKeyboard::create();
    _listener->onKeyPressed = AX_CALLBACK_2(MenuInput::onKeyPressed, this);
    _target->getEventDispatcher()->addEventListenerWithSceneGraphPriority(_listener, _target);
}

MenuInput::~MenuInput()
{
    // Tự động dọn dẹp khi Scene/Layer bị xóa
}

void MenuInput::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
    // 1. Kiểm tra nếu đối tượng điều khiển là StartScene
    auto startScene = dynamic_cast<StartScene*>(_target);
    if (startScene)
    {
        if (keyCode == EventKeyboard::KeyCode::KEY_UP_ARROW)
            startScene->moveSelectionUp();
        else if (keyCode == EventKeyboard::KeyCode::KEY_DOWN_ARROW)
            startScene->moveSelectionDown();
        else if (keyCode == EventKeyboard::KeyCode::KEY_ENTER)
            startScene->executeSelection();
        return;
    }

    // 2. Kiểm tra nếu đối tượng điều khiển là SettingsLayer
    auto settings = dynamic_cast<SettingsLayer*>(_target);
    if (settings)
    {
        event->stopPropagation();  // Chặn phím không cho lọt xuống Game
        switch (keyCode)
        {
        case EventKeyboard::KeyCode::KEY_ESCAPE:
            settings->onClose();
            break;
        case EventKeyboard::KeyCode::KEY_UP_ARROW:
            settings->navigateUp();
            break;
        case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
            settings->navigateDown();
            break;
        case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
            settings->navigateLeft();
            break;
        case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
            settings->navigateRight();
            break;
        case EventKeyboard::KeyCode::KEY_ENTER:
            settings->confirmSelection();
            break;
        }
    }
}
