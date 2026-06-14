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

    _entityConfigs = ConfigLoader::loadEntityConfig("configs/entity_config.json");

    srand((unsigned int)time(NULL));

    playerEntity = EntityFactory::create(*this, _entityConfigs.at("player"), 500, 500);

    // 1. SPAWN ORC
    int orcCount = 50;
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

        EntityFactory::create(*this, _entityConfigs.at("orc"), orcX, orcY);
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

        EntityFactory::create(*this, _entityConfigs.at("oldman"), npcX, npcY);
    }
}

void World::update(float dt, GameInput* input)
{
    // SPANNER QUÁI VẬT THEO ĐỢT
    _spawnTimer += dt;
    if (_spawnTimer >= 8.0f)  // 8 giây gen 1 lần
    {
        _spawnTimer    = 0.0f;
        int spawnCount = 3;

        for (int k = 0; k < spawnCount; k++)
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
            EntityFactory::create(*this, _entityConfigs.at("orc"), orcX, orcY);
        }
    }
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

    // 7.5. HỆ THỐNG NHẶT ĐỒ (PICKUP SYSTEM)
    if (playerEntity != -1 && transforms.count(playerEntity) && collisions.count(playerEntity))
    {
        // Lấy hộp va chạm của Player
        ax::Rect playerBox = collisions[playerEntity].getWorldHitbox(transforms[playerEntity].position);
        auto& pHealth      = healths[playerEntity];

        for (auto& [itemEntity, itemCol] : collisions)
        {
            // Chỉ kiểm tra những Entity là Item và chưa bị xóa sổ
            if (itemCol.type == CollisionType::Item && healths.count(itemEntity) && !healths[itemEntity].isDead &&
                animations.count(itemEntity))
            {
                // Lấy hộp va chạm của vật phẩm rơi trên đất
                ax::Rect itemBox = itemCol.getWorldHitbox(transforms[itemEntity].position);

                // NẾU CHẠM VÀO NHAU (NHẶT ĐƯỢC)
                if (playerBox.intersectsRect(itemBox))
                {
                    // Mẹo: Dùng đường dẫn hình ảnh để phân biệt bình máu hay bình mana
                    std::string itemName = animations[itemEntity].basePath;

                    if (itemName.find("health") != std::string::npos)  // Nếu là bình MÁU
                    {
                        int healAmount = _entityConfigs.at("health_potion").heal_hp;

                        pHealth.hp += healAmount;
                        if (pHealth.hp > pHealth.maxHp)
                            pHealth.hp = pHealth.maxHp;
                        if (hud)
                            hud->updateHP(pHealth.hp, pHealth.maxHp);
                        AXLOG("Nhat duoc Binh Mau! HP hien tai: %d", pHealth.hp);
                    }
                    else if (itemName.find("mana") != std::string::npos)  // Nếu là bình MANA
                    {
                        int healAmount = _entityConfigs.at("mana_potion").heal_mana;

                        pHealth.mana += healAmount;
                        if (pHealth.mana > pHealth.maxMana)
                            pHealth.mana = pHealth.maxMana;
                        if (hud)
                            hud->updateMana(pHealth.mana, pHealth.maxMana);
                        AXLOG("Nhat duoc Binh Mana! MP hien tai: %d", pHealth.mana);
                    }

                    // Đánh dấu vật phẩm đã chết, để Lò Dọn Rác (Bước 8) ở ngay bên dưới tự động phi tang nó
                    healths[itemEntity].isDead = true;
                }
            }
        }
    }

    // 8. HỆ THỐNG DỌN RÁC (DESTROY DEAD ENTITIES)
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
        // QUÁI CHẾT RỚT ĐỒ NGẪU NHIÊN
        if (collisions.count(e) && collisions[e].type == CollisionType::Enemy)
        {
            if (transforms.count(e))
            {
                ax::Vec2 deathPos = transforms[e].position;

                int dropChance = rand() % 100;  // Quay số từ 0 -> 99
                if (dropChance < 50)            // Tỉ lệ 50% rớt đồ
                {
                    // Lắc xúc xắc tiếp xem rớt máu hay rớt mana
                    std::string dropItem = (rand() % 2 == 0) ? "health_potion" : "mana_potion";

                    // Gọi Factory gen đồ rơi rớt ra đất            
                    EntityFactory::create(*this, _entityConfigs.at(dropItem), deathPos.x, deathPos.y);
                    AXLOG("Quai vat chet rot ra: %s!", dropItem.c_str());
                }
            }
        }
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
    }  // KẾT THÚC VÒNG LẶP XÓA Ở ĐÂY

    // [ĐÃ SỬA] IN LOG TỔNG SỐ QUÁI LIÊN TỤC MỖI FRAME 
    int currentEnemyCount = 0;
    for (auto& [e, col] : collisions)
    {
        if (col.type == CollisionType::Enemy)
            currentEnemyCount++;
    }
    AXLOG("TONG SO QUAI HIEN TAI: %d", currentEnemyCount);
}
