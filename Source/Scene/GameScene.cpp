#include "scene/GameScene.h"
#include "scene/settings/SettingsLayer.h"
#include "ecs/Systems.h"
USING_NS_AX;

bool GameScene::init()
{
    if (!Scene::init())
        return false;

    world.init(this);
    scheduleUpdate();

    _gameInput = new GameInput(this);

    _gameInput->onEscapePressed = [this]() { this->openSettings(); };
    return true;
}

void GameScene::update(float dt)
{
    // Tránh update ECS World khi đang mở Settings
    if (_isGamePaused)  return;

    Systems::UpdateInput(world, _gameInput);

    world.update(dt);
}

// Hàm public mới thay thế hoàn toàn cho onKeyPressed cũ
void GameScene::openSettings()
{
    // CHỈ xử lý mở Settings nếu game đang KHÔNG tạm dừng
    if (!_isGamePaused)
    {
        _isGamePaused = true;  // Chặn update logic của World

        // Tạo SettingsLayer
        auto settingsLayer = SettingsLayer::create(true, [this]() {
            // Lambda function sẽ chạy khi SettingsLayer gọi onClose()
            this->_isGamePaused = false;  // Cho phép World update trở lại
        });

        // Thêm vào Scene với Z-order cao để đè lên trên cùng
        this->addChild(settingsLayer, 999);
    }
}
GameScene::~GameScene()
{
    AX_SAFE_DELETE(_gameInput);
}
