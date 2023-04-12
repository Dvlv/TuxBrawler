#pragma once
#include "arena.h"
#include "brawler.h"
#include "brawlerjsonparser.h"
#include "player.h"
#include "raylib-cpp.hpp"
#include "state.h"
#include <memory>
#include <vector>

class BrawlState : public State {
    using SharedBrawler = std::shared_ptr<Brawler>;
    using Brawlers = std::vector<SharedBrawler>;

  public:
    BrawlState();

    void update() override;
    void draw() override;

    void setPlayerBrawler(BrawlerData brawlerData);
    void setArena(std::shared_ptr<Arena> arena);

  private:
    Brawlers m_brawlers;
    std::shared_ptr<Arena> m_arena;
};
