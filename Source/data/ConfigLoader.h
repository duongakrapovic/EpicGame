#pragma once
#include <string>
#include <unordered_map>

struct EntityConfig
{
    std::string type;
    std::string sprite;
    float scale;
    float speed;

    float hitbox_w = 0.0f;
    float hitbox_h = 0.0f;
    float offset_x = 0.0f;
    float offset_y = 0.0f;
};

class ConfigLoader
{
public:
    static std::unordered_map<std::string, EntityConfig> loadEntityConfig(const std::string& path);
};
