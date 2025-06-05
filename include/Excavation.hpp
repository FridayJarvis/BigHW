#pragma once
#include <vector>
#include <string>
#include "Loot.hpp"

class Excavation {
public:
    virtual std::vector<LootInfo> getPossibleLoot() const = 0;
    virtual ~Excavation() = default;
};

class ForestExcavation : public Excavation {
public:
    std::vector<LootInfo> getPossibleLoot() const override {
        return {
                {"Wood", "Hatchet", 0.8f, "assets/loot/Wood.png"},
                {"Gold", "Hammer", 0.2f, "assets/loot/Gold.png"}
        };
    }
};

class EgyptianExcavation : public Excavation {
public:
    std::vector<LootInfo> getPossibleLoot() const override {
        return {
                {"Gold", "Hammer", 0.8f, "assets/loot/Gold.png"},
                {"Diamond", "Pickaxe", 0.2f, "assets/loot/Diamond.png"}
        };
    }
};

class AfricaExcavation : public Excavation {
public:
    std::vector<LootInfo> getPossibleLoot() const override {
        return {
                {"Diamond", "Pickaxe", 0.8f, "assets/loot/Diamond.png"},
                {"Gold", "Hammer", 0.2f, "assets/loot/Gold.png"}
        };
    }
};

class MoonExcavation : public Excavation {
public:
    std::vector<LootInfo> getPossibleLoot() const override {
        return {
                {"Diamond", "Pickaxe", 0.65f, "assets/loot/Diamond.png"},
                {"Moon-dust", "Shovel", 0.35f, "assets/loot/Moon-dust.png"}
        };
    }
};