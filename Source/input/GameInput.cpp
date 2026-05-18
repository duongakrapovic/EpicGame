#include "input/GameInput.h"

USING_NS_AX;

GameInput::GameInput(ax::Node* scene) : _scene(scene), _mousePos(Vec2::ZERO)
{
    // 1. Khởi tạo Listener Bàn phím
    auto kbListener = EventListenerKeyboard::create();

    // Khi nhấn xuống -> set true
    kbListener->onKeyPressed = [this](EventKeyboard::KeyCode code, Event* event)
    {
        _keyStates[code] = true;
        // Nếu phím bấm là ESC và có hàm được đăng ký thì gọi nó chạy
        if (code == EventKeyboard::KeyCode::KEY_ESCAPE)
        {
            if (this->onEscapePressed != nullptr)
            {
                this->onEscapePressed();
            }
        }
    };

    // Khi nhả ra -> set false
    kbListener->onKeyReleased = [this](EventKeyboard::KeyCode code, Event* event) { _keyStates[code] = false; };

    // 2. Khởi tạo Listener Chuột
    auto mouseListener         = EventListenerMouse::create();
    mouseListener->onMouseMove = [this](EventMouse* event) {
        // Lấy tọa độ chuột và chuyển đổi về tọa độ World nếu cần
        _mousePos = event->getLocationInView();
    };

    // Gắn vào Scene
    _scene->getEventDispatcher()->addEventListenerWithSceneGraphPriority(kbListener, _scene);
    _scene->getEventDispatcher()->addEventListenerWithSceneGraphPriority(mouseListener, _scene);
}

GameInput::~GameInput()
{
    // Listener sẽ tự hủy cùng với Scene do dùng SceneGraphPriority
}

bool GameInput::isKeyPressed(ax::EventKeyboard::KeyCode code)
{
    // Kiểm tra trong map, nếu không có phím đó thì mặc định là false
    if (_keyStates.find(code) != _keyStates.end())
    {
        return _keyStates[code];
    }
    return false;
}
