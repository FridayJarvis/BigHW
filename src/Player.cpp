#include "Player.hpp"
#include <fstream>

bool Player::addLoot(const LootInfo &info) {
    int total = 0;
    for (const auto &[_, loot]: inventory) total += loot.count;
    if (total >= 10) return false;
    inventory[info.name].info = info;
    inventory[info.name].count++;
    return true;
}

void Player::setWalkAnimation() {
    switch (activeTool) {
        case Tool::Hammer: anim.type = AnimType::WalkHammer; break;
        case Tool::Hatchet: anim.type = AnimType::WalkHatchet; break;
        case Tool::Pickaxe: anim.type = AnimType::WalkPickaxe; break;
        case Tool::Shovel: anim.type = AnimType::WalkShovel; break;
        default: anim.type = AnimType::WalkNone; break;
    }
    anim.frameCount = 9;
    anim.frame = 0;
}

void Player::setUseAnimation() {
    switch (activeTool) {
        case Tool::Hammer: anim.type = AnimType::UseHammer; anim.frameCount = 6; break;
        case Tool::Hatchet: anim.type = AnimType::UseHatchet; anim.frameCount = 6; break;
        case Tool::Pickaxe: anim.type = AnimType::UsePickaxe; anim.frameCount = 6; break;
        case Tool::Shovel: anim.type = AnimType::UseShovel; anim.frameCount = 8; break;
        default: anim.type = AnimType::UseHands; anim.frameCount = 6; break;
    }
    anim.frame = 0;
    anim.timer = 0.f;
}

std::string Player::toolToString(const Tool& tool) {
    switch (tool) {
        case Tool::Hatchet: return "Hatchet";
        case Tool::Hammer: return "Hammer";
        case Tool::Pickaxe: return "Pickaxe";
        case Tool::Shovel: return "Shovel";
        default: return "None";
    }
}

void Player::save() {
    std::ofstream out("save.txt");
    out << money << " " << food << "\n";
    for (const auto &[item, loot]: inventory) {
        out << loot.info.name << " " << loot.count << "\n";
    }
    out.close();
}

void Player::load() {
    std::ifstream in("save.txt");
    if (!in) return;
    inventory.clear();
    in >> money >> food;
    std::string item;
    int count;
    while (in >> item >> count) {
        LootInfo dummyInfo = {item, "", 0.f, ""};
        inventory[item] = LootItem{dummyInfo, count};
    }
    in.close();
}

int Player::sellAllLoot() {
    int total = 0;

    const std::unordered_map<std::string, int> prices = {
        {"Wood", 200},
        {"Gold", 400},
        {"Diamond", 550},
        {"Moon-dust", 1000}
    };

    for (auto it = inventory.begin(); it != inventory.end();) {
        const std::string& itemName = it->first;
        int count = it->second.count;

        auto priceIt = prices.find(itemName);
        if (priceIt != prices.end()) {
            total += priceIt->second * count;
        }
        it = inventory.erase(it);
    }

    money += total;
    return total;
}