#include "data/ConfigLoader.h"
#include "axmol.h"
#include "rapidjson/document.h"

USING_NS_AX;

std::unordered_map<std::string, EntityConfig> ConfigLoader::loadEntityConfig(const std::string& path)
{
    std::unordered_map<std::string, EntityConfig> configs;
    std::string jsonStr = FileUtils::getInstance()->getStringFromFile(path);
    if (jsonStr.empty())
    {
        AXLOG("Khong the doc file config: %s", path.c_str());
        return configs;
    }

    rapidjson::Document doc;
    doc.Parse(jsonStr.c_str());
    if (doc.HasParseError() || !doc.IsObject())
    {
        AXLOG("Loi parse JSON: %s", path.c_str());
        return configs;
    }

    for (auto itr = doc.MemberBegin(); itr != doc.MemberEnd(); ++itr)
    {
        std::string key = itr->name.GetString();
        auto& obj       = itr->value;
        EntityConfig cfg;

        if (obj.HasMember("type"))
            cfg.type = obj["type"].GetString();
        if (obj.HasMember("sprite"))
            cfg.sprite = obj["sprite"].GetString();
        if (obj.HasMember("scale"))
            cfg.scale = obj["scale"].GetFloat();
        if (obj.HasMember("speed"))
            cfg.speed = obj["speed"].GetFloat();

        if (obj.HasMember("hitbox_w"))
            cfg.hitbox_w = obj["hitbox_w"].GetFloat();
        if (obj.HasMember("hitbox_h"))
            cfg.hitbox_h = obj["hitbox_h"].GetFloat();
        if (obj.HasMember("offset_x"))
            cfg.offset_x = obj["offset_x"].GetFloat();
        if (obj.HasMember("offset_y"))
            cfg.offset_y = obj["offset_y"].GetFloat();

        if (obj.HasMember("max_hp"))
            cfg.max_hp = obj["max_hp"].GetInt();
        if (obj.HasMember("max_mana"))
            cfg.max_mana = obj["max_mana"].GetInt();
        if (obj.HasMember("damage"))
            cfg.damage = obj["damage"].GetInt();
        if (obj.HasMember("frames"))
            cfg.frames = obj["frames"].GetInt();

        if (obj.HasMember("atk_len"))
            cfg.atk_len = obj["atk_len"].GetFloat();
        if (obj.HasMember("atk_thick"))
            cfg.atk_thick = obj["atk_thick"].GetFloat();
        if (obj.HasMember("atk_offset"))
            cfg.atk_offset = obj["atk_offset"].GetFloat();
        if (obj.HasMember("sprite_shift"))
            cfg.sprite_shift = obj["sprite_shift"].GetFloat();
        if (obj.HasMember("atk_cooldown"))
            cfg.atk_cooldown = obj["atk_cooldown"].GetFloat();

        configs[key] = cfg;
    }
    return configs;
}
