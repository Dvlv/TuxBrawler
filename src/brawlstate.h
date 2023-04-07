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

  private:
    Brawlers m_brawlers;
    Arena m_arena;
};
