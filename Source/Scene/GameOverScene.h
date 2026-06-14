#pragma once
#include "axmol.h"
#include <vector>

class GameOverScene : public ax::Scene
{
public:
    virtual bool init() override;
    CREATE_FUNC(GameOverScene);

    // Các hàm này bắt buộc phải public nếu MenuInput của ông gọi từ bên ngoài vào
    void moveSelectionUp();
    void moveSelectionDown();
    void executeSelection();

    // Hàm tĩnh dùng để khởi tạo Scene kèm theo điểm số
    static GameOverScene* createWithStats(int kills, int gold);

    // Biến lưu tạm
    int _finalKills = 0;
    int _finalGold  = 0;

private:
    void updateSelectionVisuals();

    std::vector<ax::Sprite*> _buttons;
    int _currentIndex = 0;

    std::vector<float> _baseScales;
};
