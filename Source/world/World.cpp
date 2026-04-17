#include "world/World.h"
#include "ecs/systems.h"
#include "data/ConfigLoader.h"
#include "ecs/EntityFactory.h"

USING_NS_AX;

Entity World::createEntity()
{
    return nextEntity++;
}

void World::init(Scene* scene)
{
    input.setup(scene);

    auto fileUtils = FileUtils::getInstance();
    fileUtils->addSearchPath("maps/chunks");
    fileUtils->addSearchPath("maps/tilesets");

    // Khởi tạo worldNode - "Cái sân khấu" chứa mọi thứ
    worldNode = ax::Node::create();
    scene->addChild(worldNode);

    // THIẾT LẬP ZOOM: Phóng to 2 lần cho chuẩn Pixel Art
    camera.setZoom(worldNode, 3.0f);

    // Load cấu hình từ JSON
    auto configs = ConfigLoader::loadEntityConfig("configs/entity_config.json");

    // Tạo player tại tọa độ (100, 100)
    // Lưu ý: EntityFactory sẽ tự động thêm Sprite vào worldNode
    playerEntity = EntityFactory::create(*this, configs["player"], 100.0f, 100.0f);

    // KHÔNG gọi loadSingleChunk nữa.
    // Hệ thống sẽ tự động load chunk phù hợp trong lần update() đầu tiên.
}

void World::update(float dt)
{
    // 1. Cập nhật Input từ bàn phím vào Component của Player
    if (playerEntity != -1 && inputs.count(playerEntity))
    {
        auto& pInput = inputs[playerEntity];
        pInput.up    = input.up;
        pInput.down  = input.down;
        pInput.left  = input.left;
        pInput.right = input.right;
    }

    // 2. Chạy các hệ thống ECS logic (Thứ tự này rất quan trọng)
    Systems::Movement(*this);       // Tính toán vị trí mới + Va chạm
    Systems::Animation(*this, dt);  // Cập nhật frame hình
    Systems::Render(*this);         // Làm tròn tọa độ và vẽ Sprite/Hitbox

    // 3. Quản lý Map và Camera
    if (playerEntity != -1 && transforms.count(playerEntity) && sprites.count(playerEntity))
    {
        auto pPos    = transforms[playerEntity].position;
        auto pSprite = sprites[playerEntity].sprite;

        // Tự động load/unload các chunk dựa trên vị trí hiện tại của Player
        // Hàm này sẽ vẽ viền đỏ và log: "Loaded Chunk X, Y"
        map.updateChunks(worldNode, pPos);

        // Cập nhật camera để worldNode di chuyển ngược hướng player (giữ player ở giữa)
        float currentZoom = worldNode->getScale();
        camera.follow(worldNode, pSprite, currentZoom);
    }
}
