#include "utils/CollisionUtils.h"

USING_NS_AX;

bool CollisionUtils::checkTileCollision(TMXTiledMap* map, const Vec2& worldPos)
{
    if (!map)
        return false;

    // Chuyển từ tọa độ thế giới sang tọa độ cục bộ của Map Node
    Vec2 localPos = worldPos - map->getPosition();

    auto mapSize  = map->getMapSize();
    auto tileSize = map->getTileSize();

    // Tính Tile Index
    int tileX = std::floor(localPos.x / tileSize.width);
    // TMX tọa độ Y bắt đầu từ trên xuống, Axmol từ dưới lên -> cần đảo ngược Y
    int tileY = std::floor((mapSize.height * tileSize.height - localPos.y) / tileSize.height);

    if (tileX < 0 || tileX >= mapSize.width || tileY < 0 || tileY >= mapSize.height)
        return false;

    // Duyệt qua tất cả các Layer
    for (auto&& child : map->getChildren())
    {
        auto layer = dynamic_cast<TMXLayer*>(child);
        if (layer)
        {
            int gid = layer->getTileGIDAt(Vec2(tileX, tileY));
            if (gid > 0)
            {
                auto props = map->getPropertiesForGID(gid);
                if (!props.isNull() && props.asValueMap().count("collision"))
                {
                    if (props.asValueMap().at("collision").asBool())
                        return true;
                }
            }
        }
    }
    return false;
}

bool CollisionUtils::checkRectCollision(TMXTiledMap* map, const Rect& boundingBox)
{
    Vec2 corners[] = {
        Vec2(boundingBox.getMinX(), boundingBox.getMinY()), Vec2(boundingBox.getMaxX(), boundingBox.getMinY()),
        Vec2(boundingBox.getMinX(), boundingBox.getMaxY()), Vec2(boundingBox.getMaxX(), boundingBox.getMaxY())};

    for (const auto& corner : corners)
    {
        if (checkTileCollision(map, corner))
            return true;
    }
    return false;
}
