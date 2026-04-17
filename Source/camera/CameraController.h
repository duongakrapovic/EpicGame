#pragma once
#include "axmol.h"

class CameraController
{
public:
    /**
     * @brief Cập nhật vị trí worldNode để đuổi theo target
     * @param worldNode Node chứa toàn bộ map và thực thể
     * @param target Thường là sprite của Player
     * @param zoom Tỉ lệ phóng to hiện tại của worldNode
     */
    void follow(ax::Node* worldNode, ax::Node* target, float zoom);

    /**
     * @brief Thiết lập mức độ phóng to cho toàn bộ thế giới
     */
    void setZoom(ax::Node* worldNode, float zoom);

private:
    float _zoom = 1.0f;  // Giá trị lưu trữ mặc định
};
