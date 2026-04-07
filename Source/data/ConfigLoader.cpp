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

        cfg.type   = it->value["type"].GetString();
        cfg.sprite = it->value["sprite"].GetString();
        cfg.scale  = it->value["scale"].GetFloat();
        cfg.speed  = it->value["speed"].GetFloat();

        result[it->name.GetString()] = cfg;
    }

    return result;
}
