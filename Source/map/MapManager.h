#pragma once
#include "axmol.h"
#include <map>

class MapManager
{
public:
    void updateChunks(ax::Node* parent, const ax::Vec2& playerPos);

    // --- Cần khai báo cả 2 hàm này để đồng bộ với file .cpp ---

    // 1. Check cho 1 điểm (dùng cho logic đơn giản)
    bool isCollision(const ax::Vec2& worldPos);
    /*Nhận vào tọa độ thế giới (World Position) của Player.

    Xác định địa chỉ: Tính toán xem tọa độ này đang nằm trong vùng không gian của Chunk nào (ví dụ: chunk 0,1 hay chunk 1,1).

    Kết nối: Kiểm tra xem Chunk đó có đang được load (_activeChunks) hay không.

    Uỷ thác: Nếu có chunk, nó sẽ "nhờ" CollisionUtils kiểm tra hộ trên chính chunk đó.*/

    // 2. Check cho Hitbox (dùng cho Player/NPC di chuyển)
    bool isCollision(const ax::Rect& boundingBox);
    /*Nhận vào một hình chữ nhật (Rect) đại diện cho Hitbox của Player.

    Chia nhỏ: Phá vỡ hình chữ nhật thành 4 điểm (4 góc: Trái dưới, Phải dưới, Trái trên, Phải trên).

    Lặp lại: Gọi hàm isCollision(Vec2) ở trên 4 lần cho 4 điểm này.

    Tác dụng: Giúp code ở Systems::Movement cực kỳ ngắn gọn.
    Thay vì bạn phải tự lấy 4 góc của nhân vật mỗi khi di chuyển,
    bạn chỉ cần ném nguyên cái Rect vào đây. Nếu bất kỳ góc nào chạm tường,
    nó sẽ trả về true (ngăn nhân vật di chuyển).*/

    static float getTileSize() { return _staticTileSize; }
    static float getChunkWidth() { return _staticChunkWidth; }
    static float getChunkHeight() { return _staticChunkHeight; }

private:
    struct Chunk
    {
        ax::TMXTiledMap* mapNode  = nullptr;
        ax::DrawNode* debugBorder = nullptr;
    };

    std::map<std::pair<int, int>, Chunk> _activeChunks;

    void loadChunk(ax::Node* parent, int x, int y);
    void removeChunk(int x, int y);

    float _chunkPixelWidth  = 0;
    float _chunkPixelHeight = 0;

    static float _staticTileSize;
    static float _staticChunkWidth;
    static float _staticChunkHeight;

    bool _initialized = false;
    // set vùng biên để load tile khác 
    float _marginHorizontal = 0.0f;  // Trái - Phải
    float _marginVertical   = 0.0f;  // Trên - Dưới

    // Hệ số x cho ngang và y cho dọc
    float _marginTileX = 13.0f;  // 10 tile trái phải
    float _marginTileY = 8.0f;   // 5 tile trên dưới
};
/* map trong axmol có dạng
* 02 12 22
  01 11 21
  00 10 12
  trục tọa độ tương tự trong toán học khi load map .
  tuy nhiên trong 1 file map thì 00 là góc trái trên như ma trận toán học 
*/
