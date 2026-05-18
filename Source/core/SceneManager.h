#pragma once
#include "axmol.h"

class SceneManager
{
public:
    static SceneManager* getInstance();

    void goToStartScene();
    void goToGameScene();
    void goToEndScene();

private:
    SceneManager();
    ~SceneManager();
    static SceneManager* _instance;
};
