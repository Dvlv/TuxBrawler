#include "brawlstate.h"
#include "arena.h"
#include "brawler.h"
#include "player.h"
#include "raylib-cpp.hpp"

using CollisionRects = std::vector<Rectangle>;

BrawlState::BrawlState() : m_arena{"map1.map"} {
    // TODO take reprs of selected brawlers and create them via Factory
    m_brawlers.push_back(
        std::make_unique<Brawler>(Rectangle{500, 500, 30, 30}, 2, 2));

    m_brawlers.push_back(
        std::make_unique<Player>(Rectangle{500, 500, 30, 30}, 2, 2));
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
