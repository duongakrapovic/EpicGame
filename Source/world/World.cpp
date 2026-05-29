#include "world/World.h"
#include "ecs/systems/Systems.h"
#include "data/ConfigLoader.h"
#include "ecs/EntityFactory.h"

USING_NS_AX;

Entity World::createEntity()
{
    return nextEntity++;
}

void World::init(Scene* scene)
{
    transforms.reserve(5000);
    sprites.reserve(5000);
    inputs.reserve(5000);
    animations.reserve(5000);
    collisions.reserve(5000);
    healths.reserve(5000);
    combats.reserve(5000);
    ais.reserve(5000);

    auto fileUtils = FileUtils::getInstance();
    fileUtils->addSearchPath("maps/chunks");
    fileUtils->addSearchPath("maps/tilesets");

    worldNode = ax::Node::create();
    scene->addChild(worldNode);
    debugDraw = ax::DrawNode::create();
    worldNode->addChild(debugDraw, 1000);

    camera.setZoom(worldNode, 3.0f);

    auto configs = ConfigLoader::loadEntityConfig("configs/entity_config.json");

    srand((unsigned int)time(NULL));

    // Tạo Player trước
    playerEntity = EntityFactory::create(*this, configs["player"], 100.0f, 100.0f);


    // 1. SPAWN ORC
    int orcCount    = 50;
    for (int k = 0; k < orcCount; k++)
    {
        float orcX = 0, orcY = 0;
        bool safeOrc = false;
        while (!safeOrc)
        {
            orcX = 16.0f + (float)(rand() % (1536 - 16));
            orcY = 16.0f + (float)(rand() % (1536 - 16));
            ax::Rect testBox(orcX - 14, orcY - 14, 28, 28);
            if (!map.isCollision(testBox))
                safeOrc = true;
        }
        EntityFactory::create(*this, configs["orc"], orcX, orcY);
    }

    // 2. SPAWN OLDMAN
    int oldmanCount = 10;
    for (int k = 0; k < oldmanCount; k++)
    {
        float npcX = 0, npcY = 0;
        bool safeNpc = false;
        while (!safeNpc)
        {
            npcX = 16.0f + (float)(rand() % (1536 - 16));
            npcY = 16.0f + (float)(rand() % (1536 - 16));
            ax::Rect testBox(npcX - 14, npcY - 14, 28, 28);
            if (!map.isCollision(testBox))
                safeNpc = true;
        }
        EntityFactory::create(*this, configs["oldman"], npcX, npcY);
    }
}

void World::update(float dt, GameInput* input)
{
    // 1. Nhận lệnh từ bàn phím
    Systems::UpdateInput(*this, input);

    // 2. AI suy nghĩ và ra lệnh bấm phím ảo cho Quái/NPC
    Systems::AI(*this, dt);

    // 3. Xử lý vung vũ khí và trừ máu
    Systems::Combat(*this, dt);

    // 4. Di chuyển và xử lý va chạm tường
    Systems::Movement(*this);

    // 5. Cập nhật frame ảnh dựa trên trạng thái (Đứng, Đi, Chém)
    Systems::Animation(*this, dt);

    // 6. Xuất mọi thứ lên màn hình
    Systems::Render(*this);

    // 7. Quản lý Map và Camera (Bám theo TỌA ĐỘ GỐC)
    if (playerEntity != -1 && transforms.count(playerEntity))
    {
        auto pPos = transforms[playerEntity].position;

        map.updateChunks(worldNode, pPos);

        float currentZoom = worldNode->getScale();
        camera.follow(worldNode, pPos, currentZoom);
    }

    // ==========================================
    // 8. HỆ THỐNG DỌN RÁC (DESTROY DEAD ENTITIES)
    // ==========================================
    std::vector<Entity> deadEntities;

    // Quét tìm những kẻ đã hết máu
    for (auto& [e, health] : healths)
    {
        if (health.isDead)
        {
            deadEntities.push_back(e);
        }
    }

    // Tiến hành xóa sổ hoàn toàn khỏi thế giới
    for (Entity e : deadEntities)
    {
        // Xóa hình ảnh khỏi màn hình Axmol
        if (sprites.count(e))
        {
            sprites[e].sprite->removeFromParent();
        }

        // Dọn sạch mọi dữ liệu liên quan trong các Component Arrays
        transforms.erase(e);
        sprites.erase(e);
        inputs.erase(e);
        animations.erase(e);
        collisions.erase(e);
        healths.erase(e);
        combats.erase(e);
        ais.erase(e);

        AXLOG("Entity [%d] da bi tieu diet va xoa khoi map!", e);
    }
}
