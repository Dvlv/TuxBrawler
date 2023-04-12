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

BrawlState::BrawlState() : State() {
    // load map from json
    // TODO this will eventually move to a map select screen

    m_brawlers.push_back(
        std::make_shared<Brawler>(Vector2{500, 500}, 2, 2, "Tux"));

    // Spawn them in and count down or something
    for (auto &brawler : m_brawlers) {
        brawler->setMovable();
        brawler->setPos({400, 400});
    }
}

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

void BrawlState::setArena(std::shared_ptr<Arena> arena) { m_arena = arena; }
