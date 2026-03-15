#ifndef __MAIN_SCENE_H__
#define __MAIN_SCENE_H__

#include "axmol.h"

class MainScene : public ax::Scene
{
public:
    bool init() override;

    // Hàm tĩnh để tạo Scene
    static ax::Scene* createScene();

    // Callback khi nhấn nút thoát
    void menuCloseCallback(ax ::Object* sender);

    // Macro tạo hàm create() tự động
    CREATE_FUNC(MainScene);
};

#endif
