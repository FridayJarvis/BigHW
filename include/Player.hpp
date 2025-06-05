#pragma once
#include <map>
#include <string>
#include <SFML/Graphics.hpp>
#include "Loot.hpp"
#include "Animation.hpp"
#include <unordered_map>

enum class Tool { None, Hatchet, Hammer, Pickaxe, Shovel };

class Player {
public:
    bool actionActive = false;
    bool actionPlaying = false;
    int money = 250;
    int food = 10;
    std::map<std::string, LootItem> inventory;
    Tool activeTool = Tool::None;
    sf::Texture sheet;
    sf::Sprite sprite;
    Animation anim;

    bool addLoot(const LootInfo &info);
    void setWalkAnimation();
    void setUseAnimation();
    static std::string toolToString(const Tool& tool);
    void save();
    void load();
    int sellAllLoot();
};