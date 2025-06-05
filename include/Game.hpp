#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include <map>
#include <string>
#include "Player.hpp"
#include "Excavation.hpp"

class Excavation;

enum class GameState {
    Base, Shop, Museum, Map, Expedition
};

class Game {
public:
    Game();
    void run();

private:
    void setupButtons();
    void handleClick(int x, int y);
    void handleMapControls(sf::Keyboard::Key key);
    void draw();
    void drawBase();
    void drawShop();
    void drawMuseum();
    void drawMap();
    void spawnLoot();
    struct LootGraphic {
        sf::Vector2f position;
        std::string type;
        sf::Sprite sprite;
    };
    LootInfo findLootInfoByName(const std::string &name) const;

    static constexpr int countLabels = 8;
    std::vector<LootGraphic> lootGraphics;
    std::map<std::string, sf::Texture> textures;
    std::map<std::string, sf::Texture> lootTextures;
    bool showInventory = false;

    sf::Texture WoodTexture, GoldTexture, DiamondTexture, MoondDustTexture, NothingTexture;
    sf::RenderWindow window;
    Player player;
    sf::Font font;
    GameState state = GameState::Base;
    std::unique_ptr<Excavation> currentExcavation;
    sf::RectangleShape buttons[countLabels];
    sf::Text buttonTexts[countLabels];
    sf::Vector2f playerPos = {100.f, 100.f};
    std::vector<sf::Vector2f> lootPoints;
    sf::Text infoText;

    const std::map<sf::Keyboard::Key, std::pair<Tool, AnimType> > toolBindings = {
        {sf::Keyboard::Num1, {Tool::Hatchet, AnimType::WalkHatchet}},
        {sf::Keyboard::Num2, {Tool::Hammer, AnimType::WalkHammer}},
        {sf::Keyboard::Num3, {Tool::Pickaxe, AnimType::WalkPickaxe}},
        {sf::Keyboard::Num4, {Tool::Shovel, AnimType::WalkShovel}},
        {sf::Keyboard::Num5, {Tool::None, AnimType::WalkNone}}
    };
};