#include "scene/GameOverScene.h"
#include "core/SceneManager.h"
#include "input/MenuInput.h"
#include "audio/AudioManager.h"
#include "scene/settings/SettingsLayer.h"

USING_NS_AX;

bool GameOverScene::init()
{
    if (!Scene::init())
        return false;

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin      = Director::getInstance()->getVisibleOrigin();

    // 1. Thêm Background
    auto bg = Sprite::create("AxmolDefault/bgEnd.png");
    if (bg)
    {
        bg->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
        bg->setScale(1280.0f / 1920.0f);  // Tỷ lệ chuẩn từ StartScene
        this->addChild(bg, 0);
    }

    // 2. Thêm Logo Game Over 
    auto logo = Sprite::create("AxmolDefault/logoGame.png");
    if (logo)
    {
        logo->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height * 0.75f + origin.y));

        logo->setScale(0.6f);
        this->addChild(logo, 1);

        //Hiệu ứng bay lơ lửng lên xuống
        auto moveUp   = MoveBy::create(1.5f, Vec2(0, 15));
        auto moveDown = moveUp->reverse();
        auto seq      = Sequence::create(EaseSineInOut::create(moveUp), EaseSineInOut::create(moveDown), nullptr);
        logo->runAction(RepeatForever::create(seq));
    }

    // 3. Khởi tạo mảng nút bấm với TỶ LỆ RIÊNG
    std::string btnFiles[] = {"AxmolDefault/btnRestart.png", "AxmolDefault/btnSettings.png", "AxmolDefault/btnExit.png"};

    // CHỈNH SIZE TỪNG NÚT Ở ĐÂY
    float customScales[] = {0.4f, 0.4f, 0.4f};

    float startY = visibleSize.height * 0.45f;
    float gapY   = 85.0f;

    for (int i = 0; i < 3; ++i)
    {
        auto btn = Sprite::create(btnFiles[i]);
        if (btn)
        {
            btn->setPosition(Vec2(visibleSize.width * 0.5f + origin.x, startY - (i * gapY)));

            // Set size riêng cho từng thằng
            btn->setScale(customScales[i]);

            this->addChild(btn, 1);
            _buttons.push_back(btn);

            // LƯU LẠI size gốc vào bộ nhớ
            _baseScales.push_back(customScales[i]);
        }
    }

    _currentIndex = 0;
    updateSelectionVisuals();


    // --- IN BẢNG ĐIỂM ---
    float textStartY = visibleSize.height * 0.45f;

    auto lblKills = Label::createWithTTF("KILLS: " + std::to_string(_finalKills), "fonts/Marker Felt.ttf", 40);
    if (lblKills)
    {
        lblKills->setPosition(Vec2(visibleSize.width * 0.25f + origin.x, textStartY));
        lblKills->setColor(Color3B::RED);
        this->addChild(lblKills, 2);
    }

    auto lblGold = Label::createWithTTF("GOLD: " + std::to_string(_finalGold), "fonts/Marker Felt.ttf", 40);
    if (lblGold)
    {
        lblGold->setPosition(Vec2(visibleSize.width * 0.25f + origin.x, textStartY - 50));  // Nằm dưới Kills 50 pixel
        lblGold->setColor(Color3B::YELLOW);
        this->addChild(lblGold, 2);
    }

    new MenuInput(this);

    // AudioManager::getInstance()->playBGM("sound/music/GameOver.wav", false);

    return true;
}

void GameOverScene::updateSelectionVisuals()
{
    for (int i = 0; i < _buttons.size(); ++i)
    {
        auto btn = _buttons[i];
        btn->stopAllActions();

        // Lôi size gốc của nút này từ bộ nhớ ra
        float baseScale = _baseScales[i];

        if (i == _currentIndex)
        {
            // Nút được chọn -> Phóng to 20% 
            btn->runAction(ScaleTo::create(0.15f, baseScale * 1.2f));
            btn->setColor(Color3B(255, 50, 50));  // Ám đỏ máu
        }
        else
        {
            // Không chọn -> Trả về chuẩn size gốc
            btn->runAction(ScaleTo::create(0.15f, baseScale));
            btn->setColor(Color3B(100, 100, 100));  // Ám xám
        }
    }
}

void GameOverScene::moveSelectionUp()
{
    if (_buttons.empty())
        return;
    _currentIndex--;
    if (_currentIndex < 0)
        _currentIndex = _buttons.size() - 1;
    updateSelectionVisuals();
}

void GameOverScene::moveSelectionDown()
{
    if (_buttons.empty())
        return;
    _currentIndex++;
    if (_currentIndex >= _buttons.size())
        _currentIndex = 0;
    updateSelectionVisuals();
}

void GameOverScene::executeSelection()
{
    if (_currentIndex == 0)  // Restart Game
    {
        SceneManager::getInstance()->goToGameScene();
    }
    else if (_currentIndex == 1)  // Settings
    {
        auto settingsLayer = SettingsLayer::create(false);
        this->addChild(settingsLayer, 100);
    }
    else if (_currentIndex == 2)  // Exit
    {
        Director::getInstance()->end();
    }
}

GameOverScene* GameOverScene::createWithStats(int kills, int gold)
{
    GameOverScene* scene = new GameOverScene();
    scene->_finalKills   = kills;  // Cất Kills vào túi
    scene->_finalGold    = gold;   // Cất Gold vào túi
    if (scene && scene->init())
    {
        scene->autorelease();
        return scene;
    }
    AX_SAFE_DELETE(scene);
    return nullptr;
}
