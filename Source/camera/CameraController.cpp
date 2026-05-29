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

void CameraController::follow(ax::Node* worldNode, ax::Vec2 targetPos, float zoom)
{
    if (!worldNode)
        return;

    // 1. Lấy tâm màn hình
    ax::Size visibleSize  = ax::Director::getInstance()->getVisibleSize();
    ax::Vec2 screenCenter = ax::Vec2(visibleSize.width / 2, visibleSize.height / 2);

    // 2. Công thức Camera cho World đã được Scale:
    // Dùng trực tiếp tọa độ lõi (targetPos) không bị rung khi Sprite bị đẩy
    ax::Vec2 newWorldPos = screenCenter - (targetPos * zoom);

    worldNode->setPosition(newWorldPos);
}
