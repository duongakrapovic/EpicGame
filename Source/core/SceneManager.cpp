#include "core/SceneManager.h"
#include "scene/StartScene.h"
#include "scene/GameScene.h"
// #include "scene/EndScene.h" // Sẽ mở comment khi bạn làm EndScene

USING_NS_AX;

SceneManager* SceneManager::_instance = nullptr;

SceneManager* SceneManager::getInstance()
{
    if (!_instance)
    {
        _instance = new SceneManager();
    }
    return _instance;
}

SceneManager::SceneManager() {}
SceneManager::~SceneManager() {}

void SceneManager::goToStartScene()
{
    auto scene    = utils::createInstance<StartScene>();
    auto director = Director::getInstance();
    if (director->getRunningScene())
    {
        director->replaceScene(TransitionFade::create(1.0f, scene, Color3B::BLACK));
    }
    else
    {
        director->runWithScene(scene);  // Chạy lần đầu khi mở game
    }
}

void SceneManager::goToGameScene()
{
    auto scene = utils::createInstance<GameScene>();
    // Chuyển cảnh mờ dần sang đen rồi sáng lên trong 1 giây
    Director::getInstance()->replaceScene(TransitionFade::create(1.0f, scene, Color3B::BLACK));
}

void SceneManager::goToEndScene()
{
    // Để trống chờ implement EndScene
}
