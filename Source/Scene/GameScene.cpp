#include "GameScene.h"
#include "GameConfig.h"
#include "InputManager/InputManager.h"

USING_NS_AX;

bool GameScene::init()
{
    if (!Scene::init())
        return false;

    auto visibleSize = _director->getVisibleSize();

    // ===== WORLD LAYER =====
    _worldLayer = Layer::create();
    this->addChild(_worldLayer);

    // ===== MAP =====
    _map = Sprite::create("map.jpg");
    _map->setAnchorPoint(Vec2(0, 0));
    _map->setPosition(Vec2(0, 0));
    _worldLayer->addChild(_map);

    // ===== PLAYER =====
    _player = Sprite::create("player.png");

    float scale = (float)TILE_SIZE / _player->getContentSize().width;
    _player->setScale(scale);

    _player->setPosition(5 * TILE_SIZE, 5 * TILE_SIZE);
    _worldLayer->addChild(_player);

    // ===== KEYBOARD =====
    auto keyListener           = EventListenerKeyboard::create();
    keyListener->onKeyPressed  = [](auto code, auto e) { InputManager::getInstance()->onKeyPressed(code); };
    keyListener->onKeyReleased = [](auto code, auto e) { InputManager::getInstance()->onKeyReleased(code); };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyListener, this);

    // ===== MOUSE =====
    auto mouseListener           = EventListenerMouse::create();
    mouseListener->onMouseDown   = [](Event* e) { InputManager::getInstance()->onMouseDown((EventMouse*)e); };
    mouseListener->onMouseUp     = [](Event* e) { InputManager::getInstance()->onMouseUp((EventMouse*)e); };
    mouseListener->onMouseMove   = [](Event* e) { InputManager::getInstance()->onMouseMove((EventMouse*)e); };
    mouseListener->onMouseScroll = [](Event* e) { InputManager::getInstance()->onMouseScroll((EventMouse*)e); };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);

    scheduleUpdate();
    return true;
}

void GameScene::update(float delta)
{
    auto input = InputManager::getInstance();

    // ===== MOVE PLAYER =====
    float speed = 5 * TILE_SIZE;
    Vec2 dir;

    if (input->isKeyDown(EventKeyboard::KeyCode::KEY_W))
        dir.y += 1;
    if (input->isKeyDown(EventKeyboard::KeyCode::KEY_S))
        dir.y -= 1;
    if (input->isKeyDown(EventKeyboard::KeyCode::KEY_A))
        dir.x -= 1;
    if (input->isKeyDown(EventKeyboard::KeyCode::KEY_D))
        dir.x += 1;

    if (dir != Vec2::ZERO)
    {
        dir.normalize();
        _player->setPosition(_player->getPosition() + dir * speed * delta);
    }

    // ===== CAMERA FOLLOW PLAYER =====
    auto visibleSize = _director->getVisibleSize();
    Vec2 screenCenter(visibleSize.width / 2, visibleSize.height / 2);

    _worldLayer->setPosition(screenCenter - _player->getPosition());

    input->endFrame();
}
