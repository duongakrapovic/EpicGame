#include "camera/CameraController.h"

USING_NS_AX;

void CameraController::setZoom(Node* worldNode, float zoom)
{
    _zoom = zoom;
    if (worldNode)
    {
        worldNode->setScale(_zoom);  // Phóng to toàn bộ worldNode
    }
}

void CameraController::follow(ax::Node* worldNode, ax::Node* target, float zoom)
{
    if (!worldNode || !target)
        return;

    // 1. Lấy vị trí thực của Player trong không gian 1.0
    ax::Vec2 playerPos = target->getPosition();

    // 2. Lấy tâm màn hình
    ax::Size visibleSize  = ax::Director::getInstance()->getVisibleSize();
    ax::Vec2 screenCenter = ax::Vec2(visibleSize.width / 2, visibleSize.height / 2);

    // 3. Công thức Camera cho World đã được Scale:
    // Khi worldNode scale lên, mọi thứ bên trong nó to lên.
    // Để Player ở giữa, ta phải dịch chuyển worldNode ngược lại một khoảng tương ứng với vị trí Player * zoom.
    ax::Vec2 newWorldPos = screenCenter - (playerPos * zoom);

    worldNode->setPosition(newWorldPos);
}
