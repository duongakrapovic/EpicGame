#pragma once
#include <string>
#include <unordered_map>

struct EntityConfig
{
    std::string type;
    std::string sprite;
    float scale = 1.0f;
    float speed = 0.0f;

    float hitbox_w = 0.0f;
    float hitbox_h = 0.0f;
    float offset_x = 0.0f;
    float offset_y = 0.0f;

    int max_hp   = 1;
    int max_stamina = 0;
    float stamina_regen = 0.0f; // thời gian hồi 1 stamina
    int max_mana = 0;
    float mana_regen = 1.0f; // thời gian hồi 1 mana
    int damage   = 0;
    int frames   = 1;

    // Các thông số chém đọc từ JSON
    float atk_cooldown;
    float atk_len      = 30.0f;
    float atk_thick    = 10.0f;
    float atk_offset   = 15.0f;
    float sprite_shift = 0.0f;
};

class ConfigLoader
{
public:
    static std::unordered_map<std::string, EntityConfig> loadEntityConfig(const std::string& path);
};
