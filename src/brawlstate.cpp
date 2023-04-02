#include "brawlstate.h"
#include "arena.h"
#include "brawler.h"
#include "player.h"
#include "raylib-cpp.hpp"
#include <fstream>
#include <nlohmann/json.hpp>

using CollisionRects = std::vector<Rectangle>;
using json = nlohmann::json;

BrawlState::BrawlState() : m_arena{"map1.map"} {

    // Load tux brawler from JSON
    // TODO this will eventually move to main or CharSelect loading screen or
    // something
    json tuxJson;
    std::ifstream tuxFile("src/resources/brawlers/tux/brawler.json");
    tuxFile >> tuxJson;

    // TODO take reprs of selected brawlers and create them via Factory
    m_brawlers.push_back(
        std::make_unique<Brawler>(Vector2{500, 500}, 2, 2, "Tux"));

    m_brawlers.push_back(std::make_unique<Player>(tuxJson));

    // Spawn them in and count down or something
    for (auto &brawler : m_brawlers) {
        brawler->setMovable();
    }
}

bool BrawlState::isFinshed() { return m_isFinished; }

void BrawlState::draw() {
    DrawText("BRAWL!", 600, 10, 20, BLACK);
    m_arena.draw();

    for (auto &brawler : m_brawlers) {
        brawler->draw();
    }
}

void BrawlState::update() {
    for (auto &brawler : m_brawlers) {
        brawler->update();
    }
}
