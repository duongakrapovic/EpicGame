#pragma once
#include "axmol.h"

class MapManager
{
public:
    void loadSingleChunk(ax::Node* parent);
    void clear();

private:
    ax::TMXTiledMap* _map = nullptr;
};
