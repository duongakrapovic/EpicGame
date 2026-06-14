#pragma once
#include "axmol.h"

class SceneManager
{
public:
    static SceneManager* getInstance();

    void goToStartScene();
    void goToGameScene();
    void goToGameOverScene(int finalKills, int finalGold);

private:
    SceneManager();
    ~SceneManager();
    static SceneManager* _instance;
};
