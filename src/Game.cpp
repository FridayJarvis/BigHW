#include "Game.hpp"
#include <cstdlib>
#include <iostream>

Game::Game() : window(sf::VideoMode(800, 600), "Paleontologist Simulator") {
    font.loadFromFile("assets/segoescb.ttf");
    player.sheet.loadFromFile("assets/spritesheet.png");
    setupButtons();
    lootPoints = {{200.f, 200.f}, {400.f, 300.f}, {600.f, 450.f}};
    infoText.setFont(font);
    infoText.setCharacterSize(16);
    infoText.setFillColor(sf::Color::White);
    infoText.setPosition(10, 560);

    GoldTexture.loadFromFile("assets/loot/Gold.png");
    DiamondTexture.loadFromFile("assets/loot/Diamond.png");
    WoodTexture.loadFromFile("assets/loot/Wood.png");
    MoondDustTexture.loadFromFile("assets/loot/Moon-dust.png");

    lootTextures["Wood"] = WoodTexture;
    lootTextures["Gold"] = GoldTexture;
    lootTextures["Diamond"] = DiamondTexture;
    lootTextures["Moon-dust"] = MoondDustTexture;

    sf::Texture hatchetTex, hammerTex, pickaxeTex, shovelTex, nothingTex;
    hatchetTex.loadFromFile("assets/tools/Hatchet.png");
    hammerTex.loadFromFile("assets/tools/Hammer.png");
    pickaxeTex.loadFromFile("assets/tools/Pickaxe.png");
    shovelTex.loadFromFile("assets/tools/Shovel.png");
    nothingTex.loadFromFile("assets/tools/Nothing.png");
    lootTextures["Hatchet"] = hatchetTex;
    lootTextures["Hammer"] = hammerTex;
    lootTextures["Pickaxe"] = pickaxeTex;
    lootTextures["Shovel"] = shovelTex;
    lootTextures["None"] = nothingTex;
}

void Game::run() {
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::MouseButtonPressed)
                handleClick(event.mouseButton.x, event.mouseButton.y);
            if (event.type == sf::Event::KeyPressed) {
                if (state == GameState::Map) {
                    handleMapControls(event.key.code);
                    if (event.key.code == sf::Keyboard::I)
                        showInventory = !showInventory;
                }
                if (event.key.code == sf::Keyboard::Escape)
                    state = GameState::Base;
            }
        }
        window.clear();
        draw();
        window.display();
    }
}

void Game::setupButtons() {
    std::string labels[countLabels] = {
        "Forest", "Egypt", "Central Africa", "Moon", "Shop", "Museum", "Save", "Load"
    };
    for (int i = 0; i < countLabels; ++i) {
        buttons[i].setSize({180.f, 40.f});
        buttons[i].setPosition(50.f, 30.f + i * 60.f);
        buttons[i].setFillColor(sf::Color::Blue);

        buttonTexts[i].setFont(font);
        buttonTexts[i].setString(labels[i]);
        buttonTexts[i].setCharacterSize(20);
        buttonTexts[i].setPosition(60.f, 35.f + i * 60.f);
    }
}

void Game::handleClick(int x, int y) {
    if (state == GameState::Base) {
        for (int i = 0; i < countLabels; ++i) {
            if (buttons[i].getGlobalBounds().contains(x, y)) {
                if (i == 0) {
                    if (player.money >= 250) {
                        player.money -= 250;
                        currentExcavation.reset(new ForestExcavation());
                        state = GameState::Map;
                        spawnLoot();
                    } else {
                        infoText.setString("You don't have enough money (250) for the Forest");
                    }
                } else if (i == 1) {
                    if (player.money >= 750) {
                        player.money -= 750;
                        currentExcavation.reset(new EgyptianExcavation());
                        state = GameState::Map;
                        spawnLoot();
                    } else {
                        infoText.setString("You don't have enough money (750) for Egypt");
                    }
                } else if (i == 2) {
                    if (player.money >= 1500) {
                        player.money -= 1500;
                        currentExcavation.reset(new AfricaExcavation());
                        state = GameState::Map;
                        spawnLoot();
                    } else {
                        infoText.setString("You don't have enough money (1500) for Central Africa");
                    }
                } else if (i == 3) {
                    if (player.money >= 3000) {
                        player.money -= 3000;
                        currentExcavation.reset(new MoonExcavation());
                        state = GameState::Map;
                        spawnLoot();
                    } else {
                        infoText.setString("You don't have enough money (3000) for the Moon");
                    }
                } else if (i == 4) {
                    state = GameState::Shop;
                } else if (i == 5) {
                    state = GameState::Museum;
                } else if (i == 6) {
                    player.save();
                } else if (i == 7) {
                    player.load();
                }
            }
        }
    } else if (state == GameState::Shop) {
        if (x >= 100 && x <= 700 && y >= 300 && y <= 340) {
            if (player.money >= 10) {
                player.money -= 10;
                player.food += 5;
            }
        } else if (x >= 100 && x <= 700 && y >= 360 && y <= 400) {
            const int earned = player.sellAllLoot();
            infoText.setString("Sold all loot for $" + std::to_string(earned));
        }
    } else if (state == GameState::Museum || state == GameState::Shop) {
        state = GameState::Base;
    }
}

void Game::handleMapControls(sf::Keyboard::Key key) {
    if (toolBindings.count(key)) {
        player.activeTool = toolBindings.at(key).first;
        player.setWalkAnimation();
    }

    constexpr float step = 5.f;
    if (key == sf::Keyboard::W) {
        playerPos.y -= step;
        player.anim.dir = Direction::Up;
    }
    if (key == sf::Keyboard::S) {
        playerPos.y += step;
        player.anim.dir = Direction::Down;
    }
    if (key == sf::Keyboard::A) {
        playerPos.x -= step;
        player.anim.dir = Direction::Left;
    }
    if (key == sf::Keyboard::D) {
        playerPos.x += step;
        player.anim.dir = Direction::Right;
    }
    player.sprite.setPosition(playerPos);

    for (size_t i = 0; i < lootGraphics.size(); ++i) {
        if (auto &loot = lootGraphics[i]; loot.sprite.getGlobalBounds().intersects(player.sprite.getGlobalBounds())) {
            if (const auto info = findLootInfoByName(loot.type);
                !info.requiredTool.empty() && info.requiredTool != player.toolToString(player.activeTool)) {
                infoText.setString("You need " + info.requiredTool + "!");
            } else if (!player.addLoot(info)) {
                infoText.setString("Inventory is full!");
            } else {
                player.actionPlaying = true;
                player.setUseAnimation();
                player.food--;
                infoText.setString("Found " + loot.type + "!");
                lootGraphics.erase(lootGraphics.begin() + i);
            }
            break;
        }
    }
}

void Game::draw() {
    switch (state) {
        case GameState::Base:
            drawBase();
            break;
        case GameState::Shop:
            drawShop();
            break;
        case GameState::Museum:
            drawMuseum();
            break;
        case GameState::Map:
            drawMap();
            break;
        default:
            break;
    }
}

void Game::drawBase() {
    for (int i = 0; i < countLabels; ++i) {
        window.draw(buttons[i]);
        window.draw(buttonTexts[i]);
    }
    sf::Text stats("Money: " + std::to_string(player.money) + ", Food: " +
                   std::to_string(player.food), font, 20);
    stats.setPosition(250.f, 20.f);
    window.draw(stats);

    window.draw(infoText);
}

void Game::drawShop() {
    sf::Text buyMsg("Click to buy food for 10$ (ESC to exit)", font, 24);
    buyMsg.setPosition(100.f, 300.f);
    window.draw(buyMsg);

    sf::Text sellMsg("Click to sell all loot", font, 24);
    sellMsg.setPosition(100.f, 360.f);
    window.draw(sellMsg);
}

void Game::drawMuseum() {
    int y = 20;
    sf::Sprite lootSprite;
    sf::Text text;
    float desiredSize = 64.f;
    for (const auto &[item, loot]: player.inventory) {
        if (lootTextures.count(item)) {
            sf::Sprite lootSprite;
            lootSprite.setTexture(lootTextures[item]);
            float scaleX = desiredSize / lootTextures[item].getSize().x;
            float scaleY = desiredSize / lootTextures[item].getSize().y;
            lootSprite.setScale(scaleX, scaleY);
            lootSprite.setPosition(450.f, static_cast<float>(y) - 15.f);
            window.draw(lootSprite);
        }

        sf::Text text;
        text.setFont(font);
        text.setCharacterSize(20);
        text.setString(item + ": " + std::to_string(loot.count));
        text.setPosition(300.f, static_cast<float>(y));
        window.draw(text);
        y += 50;
    }
    sf::Text tip("ESC to return to Base", font, 18);
    tip.setPosition(10.f, 560.f);
    window.draw(tip);
}

void Game::drawMap() {
    for (const auto &loot: lootGraphics) {
        window.draw(loot.sprite);
    }

    static sf::Clock animClock;
    float frameTime = 0.1f;
    player.anim.timer += animClock.restart().asSeconds();

    if (player.actionPlaying) {
        if (player.anim.timer >= frameTime) {
            player.anim.timer = 0.f;
            player.anim.frame++;
            if (player.anim.frame >= player.anim.frameCount) {
                player.actionPlaying = false;
                player.setWalkAnimation();
            }
        }
    } else {
        if (player.anim.timer >= frameTime) {
            player.anim.timer = 0.f;
            player.anim.frame = (player.anim.frame + 1) % player.anim.frameCount;
        }
    }

    const AnimInfo &info = animInfos.at(player.anim.type);
    int frameWidth = 64, frameHeight = 64;
    int row = info.baseRow + static_cast<int>(player.anim.dir);
    player.anim.frameCount = info.frameCount;

    player.sprite.setTexture(player.sheet);
    player.sprite.setTextureRect(sf::IntRect(
        player.anim.frame * frameWidth,
        row * frameHeight,
        frameWidth, frameHeight
    ));
    player.sprite.setPosition(playerPos);
    window.draw(player.sprite);

    sf::Text stats("Money: " + std::to_string(player.money) + "  Food: " +
                   std::to_string(player.food), font, 18);
    stats.setPosition(10.f, 10.f);
    window.draw(stats);

    sf::Text tip("WASD to move | ESC - Back | I - Inventory", font, 16);
    tip.setPosition(10.f, 40.f);
    window.draw(tip);

    window.draw(infoText);

    sf::Text toolTip("For choice tool: 1-4", font, 16);
    toolTip.setPosition(10.f, 70.f);
    window.draw(toolTip);

    std::vector<std::pair<Tool, std::string> > toolList = {
        {Tool::Hatchet, "Hatchet"},
        {Tool::Hammer, "Hammer"},
        {Tool::Pickaxe, "Pickaxe"},
        {Tool::Shovel, "Shovel"},
        {Tool::None, "None"}
    };
    int tx = 200, ty = 65;
    float desiredSize = 64.f;
    for (int i = 0; i < toolList.size(); ++i) {
        if (lootTextures.count(toolList[i].second)) {
            sf::Sprite icon(lootTextures[toolList[i].second]);
            float scaleX, scaleY;
            scaleX = desiredSize / icon.getTexture()->getSize().x;
            scaleY = desiredSize / icon.getTexture()->getSize().y;
            if (toolList[i].second == "None") {
                scaleX = 48.f / icon.getTexture()->getSize().x;
                scaleY = 48.f / icon.getTexture()->getSize().y;
            }

            icon.setScale(scaleX, scaleY);
            icon.setPosition(static_cast<float>(tx + i * 70), static_cast<float>(ty));
            window.draw(icon);
            sf::Text num(std::to_string(i + 1), font, 14);
            num.setPosition(static_cast<float>(tx + i * 70), static_cast<float>(ty + 65));
            window.draw(num);
        }
    }

    std::string currentTool;
    for (auto &[tool, name]: toolList)
        if (player.activeTool == tool) currentTool = name;

    if (!currentTool.empty() && lootTextures.count(currentTool)) {
        sf::Text inHand("In the hands:", font, 16);
        inHand.setPosition(10.f, 100.f);
        window.draw(inHand);
        sf::Sprite handIcon(lootTextures[currentTool]);
        float scaleX, scaleY;
        scaleX = 64.f / handIcon.getTexture()->getSize().x;
        scaleY = 64.f / handIcon.getTexture()->getSize().y;
        if (currentTool == "None") {
            scaleX = 48.f / handIcon.getTexture()->getSize().x;
            scaleY = 48.f / handIcon.getTexture()->getSize().y;
        }
        handIcon.setScale(scaleX, scaleY);
        handIcon.setPosition(120.f, 85.f);
        window.draw(handIcon);
    }

    if (showInventory) {
        int totalItems = 0;
        for (const auto &[_, loot]: player.inventory) {
            totalItems += loot.count;
        }

        int itemCount = static_cast<int>(player.inventory.size());
        float panelHeight = std::max(60.f + itemCount * 50.f, 100.f);
        sf::RectangleShape panel({220.f, panelHeight});
        panel.setFillColor(sf::Color(50, 50, 50, 200));
        panel.setPosition(560.f, 10.f);
        window.draw(panel);

        sf::Text invTitle("Inventory", font, 16);
        invTitle.setPosition(570.f, 15.f);
        window.draw(invTitle);

        int x = 570;
        int y = 40;
        float desiredSize = 50.f;
        for (const auto &[item, loot]: player.inventory) {
            if (lootTextures.count(item)) {
                sf::Sprite icon(lootTextures[item]);
                float scaleFactor = desiredSize / icon.getTexture()->getSize().x;
                icon.setScale(scaleFactor, scaleFactor);
                icon.setPosition(static_cast<float>(x), static_cast<float>(y));
                window.draw(icon);
            }

            sf::Text countText("x" + std::to_string(loot.count), font, 12);
            countText.setPosition(static_cast<float>(x + 50), static_cast<float>(y + 32));
            window.draw(countText);

            y += 50;
        }

        sf::Text cap("Capacity: " + std::to_string(totalItems) + "/10", font, 14);
        cap.setPosition(570, 140);
        window.draw(cap);
    }
}

void Game::spawnLoot() {
    lootGraphics.clear();
    if (!currentExcavation) return;

    const int count = 3 + std::rand() % 4;
    for (int i = 0; i < count; ++i) {
        float roll = static_cast<float>(std::rand()) / RAND_MAX;
        float accum = 0.f;
        LootInfo chosen;
        for (auto &info: currentExcavation->getPossibleLoot()) {
            accum += info.chance;
            if (roll <= accum) {
                chosen = info;
                break;
            }
        }
        if (!lootTextures.count(chosen.name)) {
            sf::Texture tex;
            tex.loadFromFile(chosen.texturePath);
            lootTextures[chosen.name] = tex;
        }
        LootGraphic lg;
        lg.type = chosen.name;
        lg.position = {
            100.f + static_cast<float>(std::rand() % 600),
            100.f + static_cast<float>(std::rand() % 400)
        };
        lg.sprite.setTexture(lootTextures[chosen.name]);
        float desiredSize = 50.f;
        float scaleFactor = desiredSize / lg.sprite.getTexture()->getSize().x;
        lg.sprite.setScale(scaleFactor, scaleFactor);
        lg.sprite.setPosition(lg.position);
        lootGraphics.push_back(lg);
    }
}

LootInfo Game::findLootInfoByName(const std::string &name) const {
    if (!currentExcavation) return {name, "", 0.f, ""};
    for (const auto &info: currentExcavation->getPossibleLoot()) {
        if (info.name == name) return info;
    }
    return {name, "", 0.f, ""};
}
