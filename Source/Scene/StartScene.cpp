#include "scene/StartScene.h"
#include "core/SceneManager.h"
#include "scene/settings/SettingsLayer.h"
#include "input/MenuInput.h" 

USING_NS_AX;

bool StartScene::init()
{
    if (!Scene::init())
        return false;

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin      = Director::getInstance()->getVisibleOrigin();

    // 1. Thêm Background
    auto bg = Sprite::create("AxmolDefault/bgStart.png");
    if (bg)
    {
        bg->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
        bg->setScale(1280.0f / 1920.0f);
        this->addChild(bg, 0);
    }

    // 2. Thêm Logo với hiệu ứng bay lơ lửng
    auto logo = Sprite::create("AxmolDefault/logoGame.png");
    if (logo)
    {
        logo->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height * 0.75f + origin.y));
        logo->setScale(0.6f);
        this->addChild(logo, 1);

        auto moveUp   = MoveBy::create(1.5f, Vec2(0, 15));
        auto moveDown = moveUp->reverse();
        auto seq      = Sequence::create(EaseSineInOut::create(moveUp), EaseSineInOut::create(moveDown), nullptr);
        logo->runAction(RepeatForever::create(seq));
    }

    // 3. Khởi tạo các nút bấm
    std::string btnFiles[] = {"AxmolDefault/btnStart.png", "AxmolDefault/btnSettings.png", "AxmolDefault/btnExit.png"};

    float startY = visibleSize.height * 0.45f;
    float gapY   = 85.0f;

    for (int i = 0; i < 3; ++i)
    {
        auto btn = Sprite::create(btnFiles[i]);
        if (btn)
        {
            btn->setPosition(Vec2(visibleSize.width / 2 + origin.x, startY - (i * gapY)));
            btn->setScale(SCALE_NORMAL);
            this->addChild(btn, 1);
            _buttons.push_back(btn);
        }
    }

    _currentIndex = 0;
    updateSelectionVisuals();

    new MenuInput(this);

    return true;
}

void StartScene::updateSelectionVisuals()
{
    for (int i = 0; i < _buttons.size(); ++i)
    {
        auto btn = _buttons[i];
        btn->stopAllActions();

        if (i == _currentIndex)
        {
            btn->runAction(ScaleTo::create(0.15f, SCALE_SELECTED));
            btn->setColor(Color3B(255, 255, 255));
        }
        else
        {
            btn->runAction(ScaleTo::create(0.15f, SCALE_NORMAL));
            btn->setColor(Color3B(200, 200, 200));
        }
    }
}

// Tách logic phím Lên thành hàm độc lập
void StartScene::moveSelectionUp()
{
    if (_buttons.empty())
        return;
    _currentIndex--;
    if (_currentIndex < 0)
        _currentIndex = _buttons.size() - 1;
    updateSelectionVisuals();
}

// Tách logic phím Xuống thành hàm độc lập
void StartScene::moveSelectionDown()
{
    if (_buttons.empty())
        return;
    _currentIndex++;
    if (_currentIndex >= _buttons.size())
        _currentIndex = 0;
    updateSelectionVisuals();
}

void StartScene::executeSelection()
{
    if (_currentIndex == 0)  // Start
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
