#include "brawlstate.h"
#include "arena.h"
#include "brawler.h"
#include "brawlerjsonparser.h"
#include "player.h"
#include "raylib-cpp.hpp"
#include "state.h"
#include <fstream>
#include <memory>
#include <nlohmann/json.hpp>

using CollisionRects = std::vector<Rectangle>;
using SharedBrawler = std::shared_ptr<Brawler>;
using json = nlohmann::json;

BrawlState::BrawlState() : State() {}

void BrawlState::draw() {
    DrawText("BRAWL!", 600, 10, 20, BLACK);
    m_arena->draw();

    for (auto &brawler : m_brawlers) {
        brawler->draw();
    }
}

void BrawlState::update() {
    // TODO this shouldnt be copied on every update, maybe store this in the
    // brawlstate itself
    CollisionRects mapCollisions = m_arena->getCollisionRects();

    for (auto &brawler : m_brawlers) {
        brawler->update(mapCollisions);
    }
}

void BrawlState::setPlayerBrawler(BrawlerData brawlerData) {
    std::shared_ptr<Player> player = std::make_shared<Player>(brawlerData);
    player->setPos({300, 300});
    player->setMovable();
    player->loadSpritesheets();

    m_brawlers.push_back(player);
}

void BrawlState::addCpuBrawler(BrawlerData brawlerData) {
    std::shared_ptr<Brawler> brawler = std::make_shared<Brawler>(brawlerData);
    brawler->setPos({600, 300});
    brawler->setMovable();
    brawler->loadSpritesheets();

    m_brawlers.push_back(brawler);
}

void BrawlState::setArena(std::shared_ptr<Arena> arena) { m_arena = arena; }
