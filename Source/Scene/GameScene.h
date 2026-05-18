#pragma once
#include "axmol.h"
#include "world/World.h"
#include "input/GameInput.h"

class SettingsLayer;

class GameScene : public ax::Scene
{
public:
    virtual bool init();
    void update(float dt) override;
     ~GameScene() override;

    CREATE_FUNC(GameScene);

    // MỞ KHÓA PUBLIC: Để file GameInput sắp tạo có quyền gọi mở Settings
    void openSettings();

private:
    // Đã xóa hoàn toàn hàm onKeyPressed ở đây

    World world;
    GameInput* _gameInput = nullptr; //Quản lý input gameplay
   
    bool _isGamePaused = false;  // Biến cờ kiểm tra trạng thái pause
};
