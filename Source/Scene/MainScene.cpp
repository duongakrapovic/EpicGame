#include "Scene/MainScene.h"
#include "Scene/GameScene.h"  //Phải có file này để gọi GameScene

USING_NS_AX;

Scene* MainScene::createScene()
{
    return MainScene::create();
}

bool MainScene::init()
{
    if (!Scene::init())
    {
        return false;
    }

    auto visibleSize = _director->getVisibleSize();// kích thước màn hình 
    auto origin      = _director->getVisibleOrigin();// điểm tọa độ gốc - trái dưới

    // 1. Tạo Menu để chứa các nút bấm
    auto menu = Menu::create();
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    // 2. Nút START GAME (Dùng để chuyển sang GameScene)
    auto startLabel = Label::createWithTTF("START GAME", "fonts/Marker Felt.ttf", 40);
    auto startItem  = MenuItemLabel::create(startLabel, [this](Object* sender) {
        // Chuyển cảnh sang GameScene
        auto scene = utils::createInstance<GameScene>();
        _director->replaceScene(TransitionFade::create(0.5f, scene));
    });
    startItem->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 - 40));
    menu->addChild(startItem);

    // 3. Nút EXIT (Nút thoát mặc định)
    auto closeLabel = Label::createWithTTF("ESC", "fonts/Marker Felt.ttf", 40);
    auto closeItem = MenuItemLabel::create(
        closeLabel,
        AX_CALLBACK_1(MainScene::menuCloseCallback, this));

    closeItem->setPosition(Vec2(origin.x + closeItem->getContentSize().width / 2 + 10,
                                origin.y + visibleSize.height - closeItem->getContentSize().height / 2 - 10)
    );
    menu->addChild(closeItem);

    // 4. Logo HelloWorld mặc định
    auto sprite = Sprite::create("HelloWorld.png");
    sprite->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y + 100));
    this->addChild(sprite, 0);

    return true;
}

void MainScene::menuCloseCallback(Object* sender)
{
    _director->end();
}
