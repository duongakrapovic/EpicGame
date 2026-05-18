#include "data/ConfigLoader.h"
#include "axmol.h"
#include "rapidjson/document.h"

using namespace rapidjson;

std::unordered_map<std::string, EntityConfig> ConfigLoader::loadEntityConfig(const std::string& path)
{
    std::unordered_map<std::string, EntityConfig> result;

    std::string content = ax::FileUtils::getInstance()->getStringFromFile(path);

    Document doc;
    doc.Parse(content.c_str());

    for (auto it = doc.MemberBegin(); it != doc.MemberEnd(); ++it)
    {
        EntityConfig cfg;

        // Các thông số cơ bản (Bắt buộc phải có)
        cfg.type   = it->value["type"].GetString();
        cfg.sprite = it->value["sprite"].GetString();

        // Dùng HasMember để chống Crash nếu quên ghi vào JSON
        cfg.scale = it->value.HasMember("scale") ? it->value["scale"].GetFloat() : 1.0f;
        cfg.speed = it->value.HasMember("speed") ? it->value["speed"].GetFloat() : 0.0f;

        // --- BỔ SUNG ĐỌC THÔNG SỐ HITBOX TỪ JSON ---
        cfg.hitbox_w = it->value.HasMember("hitbox_w") ? it->value["hitbox_w"].GetFloat() : 12.0f;
        cfg.hitbox_h = it->value.HasMember("hitbox_h") ? it->value["hitbox_h"].GetFloat() : 14.0f;
        cfg.offset_x = it->value.HasMember("offset_x") ? it->value["offset_x"].GetFloat() : 0.0f;
        cfg.offset_y = it->value.HasMember("offset_y") ? it->value["offset_y"].GetFloat() : 0.0f;

        result[it->name.GetString()] = cfg;
    }

    return result;
}
