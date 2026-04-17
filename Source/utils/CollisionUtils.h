#pragma once
#include "axmol.h"
// logic về check collision, tập trung cho 1 file map hiện thời .
/*Nhận vào một con trỏ TMXTiledMap và một tọa độ.

Tính toán xem tọa độ đó thuộc về ô (Tile) nào trong Map.

Truy cập vào các Layer, đọc chỉ số GID của Tile đó.

Kiểm tra thuộc tính (Properties) xem Tile đó có được đánh dấu là collision = true hay không.
*/ 
class CollisionUtils
{
public:
    /**
     * @brief Kiểm tra xem một tọa độ thế giới có chạm vào Tile có thuộc tính 'collision' không
     */
    static bool checkTileCollision(ax::TMXTiledMap* map, const ax::Vec2& worldPos);

    /**
     * @brief Kiểm tra va chạm cho một hình chữ nhật (Hitbox)
     */
    static bool checkRectCollision(ax::TMXTiledMap* map, const ax::Rect& boundingBox);
};
