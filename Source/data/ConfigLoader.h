#pragma once
#include <string>
#include <unordered_map>

struct EntityConfig
{
    std::string type;
    std::string sprite;
    float scale;
    float speed;
};

class ConfigLoader
{
public:
    static std::unordered_map<std::string, EntityConfig> loadEntityConfig(const std::string& path);
};
