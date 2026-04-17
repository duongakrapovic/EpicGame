#include "map/MapManager.h"
#include "utils/CollisionUtils.h"
#include <cmath>
#include <set>

USING_NS_AX;

// 1. KHỞI TẠO CÁC BIẾN STATIC
float MapManager::_staticTileSize    = 0;
float MapManager::_staticChunkWidth  = 0;
float MapManager::_staticChunkHeight = 0;

void MapManager::loadChunk(Node* parent, int x, int y)
{
    if (_activeChunks.count({x, y}))
        return;

    std::string path = StringUtils::format("maps/chunks/chunk%d_%d.tmx", x, y);

    // Kiểm tra file tồn tại để tránh crash
    if (!FileUtils::getInstance()->isFileExist(path))
        return;

    auto map = TMXTiledMap::create(path);

    if (map)
    {
        // 2. TỰ ĐỘNG KHỞI TẠO KÍCH THƯỚC (Chỉ chạy 1 lần ở chunk đầu tiên)
        if (!_initialized)
        {
            auto mapSize  = map->getMapSize();   // Ví dụ: 32x32 tiles
            auto tileSize = map->getTileSize();  // Ví dụ: 16x16 pixels

            // Gán cho biến local của instance
            _chunkPixelWidth  = mapSize.width * tileSize.width;
            _chunkPixelHeight = mapSize.height * tileSize.height;

            // GÁN CHO CÁC BIẾN STATIC ĐỂ CÁC FILE KHÁC GỌI ĐƯỢC
            _staticTileSize    = tileSize.width;
            _staticChunkWidth  = _chunkPixelWidth;
            _staticChunkHeight = _chunkPixelHeight;

            // TÍNH TOÁN MARGIN RIÊNG BIỆT
            _marginHorizontal = _staticTileSize * _marginTileX;  // 13 * 16px = 160px
            _marginVertical   = _staticTileSize * _marginTileY;  // 8 * 16px = 80px

            _initialized = true;
            AXLOG(" Chunk Size Initialized : % .1f x % .1f | Tile : % .1f ",
                _chunkPixelWidth, _chunkPixelHeight,_staticTileSize);
        }

        // Khử nhòe cho Pixel Art
        for (auto&& child : map->getChildren())
        {
            auto texProto = dynamic_cast<ax::TextureProtocol*>(child);
            if (texProto && texProto->getTexture())
                texProto->getTexture()->setAliasTexParameters();
        }

        // Sét vị trí dựa trên kích thước thực tế
        map->setPosition(Vec2(x * _chunkPixelWidth, y * _chunkPixelHeight));
        parent->addChild(map, -1);

        // Viền Debug đỏ để nhìn thấy ranh giới chunk
        auto border     = DrawNode::create();
        Vec2 vertices[] = {
            Vec2(0, 0), Vec2(_chunkPixelWidth, 0),
            Vec2(_chunkPixelWidth, _chunkPixelHeight),
            Vec2(0, _chunkPixelHeight)
        };
        border->drawPoly(vertices, 4, true, Color4F(1.0f, 0.0f, 0.0f, 1.0f));
        map->addChild(border, 100);

        _activeChunks[{x, y}] = {map, border};
        AXLOG("Loaded Chunk: %d, %d", x, y);
    }
}

void MapManager::removeChunk(int x, int y)
{
    if (_activeChunks.count({x, y}))
    {
        _activeChunks[{x, y}].mapNode->removeFromParent();
        _activeChunks.erase({x, y});
        AXLOG("Removed Chunk: %d, %d", x, y);
    }
}

void MapManager::updateChunks(Node* parent, const Vec2& playerPos)
{
    // Nếu chưa load chunk nào thì thử load chunk 0,0 để lấy thông số
    if (!_initialized)
    {
        loadChunk(parent, 0, 0);
    }
    if (!_initialized)
        return;

    // Xác định Player đang đứng ở Chunk nào
    int currentX = std::floor(playerPos.x / _chunkPixelWidth);
    int currentY = std::floor(playerPos.y / _chunkPixelHeight);

    // Tính toán localX, localY dựa trên kích thước thực
    float localX = playerPos.x - (currentX * _chunkPixelWidth);
    float localY = playerPos.y - (currentY * _chunkPixelHeight);

    std::set<std::pair<int, int>> required;
    required.insert({currentX, currentY});

    // Kiểm tra biên để load chunk lân cận
        // Kiểm tra biên Trái/Phải dùng _marginHorizontal
        bool nearRight = (localX > _chunkPixelWidth - _marginHorizontal);
        bool nearLeft  = (localX < _marginHorizontal);

        // Kiểm tra biên Trên/Dưới dùng _marginVertical
        bool nearTop = (localY > _chunkPixelHeight - _marginVertical);
        bool nearBot = (localY < _marginVertical);

    if (nearRight)
        required.insert({currentX + 1, currentY});
    if (nearLeft)
        required.insert({currentX - 1, currentY});
    if (nearTop)
        required.insert({currentX, currentY + 1});
    if (nearBot)
        required.insert({currentX, currentY - 1});

    // Nếu ở góc (biên của 2 cạnh)
    if (nearRight && nearTop)
        required.insert({currentX + 1, currentY + 1});
    if (nearRight && nearBot)
        required.insert({currentX + 1, currentY - 1});
    if (nearLeft && nearTop)
        required.insert({currentX - 1, currentY + 1});
    if (nearLeft && nearBot)
        required.insert({currentX - 1, currentY - 1});

    // 3. Load chunk mới cần thiết
    for (auto& coord : required)
    {
        loadChunk(parent, coord.first, coord.second);
    }

    // 4. Xóa chunk thừa
    for (auto it = _activeChunks.begin(); it != _activeChunks.end();)
    {
        // Nếu tọa độ chunk hiện tại KHÔNG nằm trong danh sách cần thiết (required)
        if (required.find(it->first) == required.end())
        {
            // GỌI HÀM NÀY: Để kích hoạt AXLOG xóa và dọn dẹp Node
            int x = it->first.first;
            int y = it->first.second;

            // Quan trọng: Phải tăng iterator TRƯỚC khi xóa để tránh crash map
            it++;
            removeChunk(x, y);
        }
        else
            ++it;
    }
}

bool MapManager::isCollision(const Vec2& worldPos)
{
    if (!_initialized)
        return false;

    // 1. Tìm chunk chứa worldPos
    int cx = std::floor(worldPos.x / _chunkPixelWidth);
    int cy = std::floor(worldPos.y / _chunkPixelHeight);

    if (_activeChunks.count({cx, cy}))
    {
        // 2. Gọi hàm từ file Utils đã tách
        return CollisionUtils::checkTileCollision(_activeChunks[{cx, cy}].mapNode, worldPos);
    }
    return false;
}

//Gọi ngược lại chính hàm isCollision(Vec2) của nó 4 lần cho 4 góc.
bool MapManager::isCollision(const ax::Rect& boundingBox)
{
    // Lấy 4 góc của Hitbox
    ax::Vec2 corners[] = {
        ax::Vec2(boundingBox.getMinX(), boundingBox.getMinY()), ax::Vec2(boundingBox.getMaxX(), boundingBox.getMinY()),
        ax::Vec2(boundingBox.getMinX(), boundingBox.getMaxY()), ax::Vec2(boundingBox.getMaxX(), boundingBox.getMaxY())};

    for (const auto& corner : corners)
    {
        // Gọi lại hàm isCollision(Vec2) ở ngay phía trên
        if (this->isCollision(corner))
            return true;
    }
    return false;
}
