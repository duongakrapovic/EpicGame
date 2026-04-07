#include "camera/CameraController.h"

USING_NS_AX;

void CameraController::follow(Node* worldNode, Node* target)
{
    Vec2 targetPos    = target->getPosition();
    Vec2 screenCenter = Director::getInstance()->getVisibleSize() / 2;

    Vec2 offset = screenCenter - targetPos;

    worldNode->setPosition(offset);
}
