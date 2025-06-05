#pragma once
#include <string>

struct LootInfo {
    std::string name;
    std::string requiredTool;
    float chance;
    std::string texturePath;
};

struct LootItem {
    LootInfo info;
    int count = 0;
};