#include "map/MapManager.h"

USING_NS_AX;

void MapManager::loadSingleChunk(Node* parent)
{
    if (!parent)
        return;

    // tránh load lại nhiều lần
    if (_map)
        return;

    std::string tmxPath = "maps/chunks/chunk0_0.tmx";

    AXLOG("Loading map: %s", tmxPath.c_str());

    _map = TMXTiledMap::create(tmxPath);

    if (_map)
    {
        _map->setPosition(Vec2(0, 0));
        parent->addChild(_map, -1);
        AXLOG("✅ Map loaded SUCCESS");
    }
    else
    {
        AXLOG("❌ Map load FAILED");
    }
}

void MapManager::clear()
{
    if (_map)
    {
        _map->removeFromParent();
        _map = nullptr;
    }
}
