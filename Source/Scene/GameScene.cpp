#include "scene/GameScene.h"
#include "scene/settings/SettingsLayer.h"
#include "ecs/systems/Systems.h"
#include "audio/AudioManager.h"
#include "core/SceneManager.h"
#include "ui/HUD.h"
USING_NS_AX;

bool GameScene::init()
{
    if (!Scene::init())
        return false;

    world.init(this);
    // --- KHỞI TẠO HUD ---
    auto hud = HUD::create();
    if (hud)
    {
        this->addChild(hud, 100);
        world.hud = hud;

        // Ngay khi có HUD, lấy máu gốc của Player đổ lên màn hình
        if (world.healths.count(world.playerEntity))
        {
            auto& pHealth = world.healths[world.playerEntity];
            hud->updateHP(pHealth.hp, pHealth.maxHp);
            hud->updateStamina(pHealth.stamina, pHealth.maxStamina);
            hud->updateMana(pHealth.mana, pHealth.maxMana);
        }
    }
    scheduleUpdate();

    _gameInput = new GameInput(this);

    _gameInput->onEscapePressed = [this]() { this->openSettings(); };

    AudioManager::getInstance()->playBGM("sound/music/BlueBoyAdventure.wav", true);
    return true;
}

void GameScene::update(float dt)
{
    // Tránh update ECS World khi đang mở Settings
    if (_isGamePaused)
        return;

    // [ĐÃ SỬA] Chạy update tổng của World và truyền thẳng GameInput vào
    world.update(dt, _gameInput);

    // KIỂM TRA SINH TỬ
    // Nếu hệ thống dọn rác đã xóa Player -> Game Over
    if (world.playerEntity != -1 && world.transforms.count(world.playerEntity) == 0)
    {
        _isGamePaused = true;

        // 1. Mở sổ của HUD ra xem tổng kết
        int finalKills = 0;
        int finalGold  = 0;
        if (world.hud)
        {
            finalKills = world.hud->getKillCount();
            finalGold  = world.hud->getGoldCount();
        }

        // 2. Giao số liệu cho SceneManager chở đi sang màn Game Over
        SceneManager::getInstance()->goToGameOverScene(finalKills, finalGold);
        return;
    }

    // Kiểm tra xem người chơi có đang giữ bất kỳ phím di chuyển nào không (WASD)
    bool isMoving = _gameInput->isKeyPressed(ax::EventKeyboard::KeyCode::KEY_W) ||
                    _gameInput->isKeyPressed(ax::EventKeyboard::KeyCode::KEY_S) ||
                    _gameInput->isKeyPressed(ax::EventKeyboard::KeyCode::KEY_A) ||
                    _gameInput->isKeyPressed(ax::EventKeyboard::KeyCode::KEY_D);

    if (isMoving)
    {
        _footstepTimer += dt;
        if (_footstepTimer >= 0.4f)
        {
            AudioManager::getInstance()->playSFX("sound/sfx/stairs.wav");
            _footstepTimer = 0.0f;
        }
    }
    else
    {
        // Khi nhả phím, reset lại để lần bấm tiếp theo phát tiếng ngay lập tức
        _footstepTimer = 0.4f;
    }
}

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
