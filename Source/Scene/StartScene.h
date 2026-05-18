#pragma once
#include "axmol.h"
#include <vector>

class StartScene : public ax::Scene
{
public:
    virtual bool init() override;
    CREATE_FUNC(StartScene);

    // MỞ KHÓA PUBLIC: Để file MenuInput sắp tạo có quyền gọi vào đây điều khiển
    void moveSelectionUp();
    void moveSelectionDown();
    void executeSelection();

private:
    void updateSelectionVisuals();

    std::vector<ax::Sprite*> _buttons;
    int _currentIndex;  // 0: Start, 1: Settings, 2: Exit

    // Hằng số scale để dễ tinh chỉnh
    const float SCALE_NORMAL   = 0.4f;
    const float SCALE_SELECTED = 0.46f;
};
