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

private:
    void updateSelectionVisuals();

    std::vector<ax::Sprite*> _buttons;
    int _currentIndex = 0;

    std::vector<float> _baseScales;
};
