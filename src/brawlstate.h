#pragma once
#include "arena.h"
#include "brawler.h"
#include "raylib-cpp.hpp"
#include "state.h"
#include <memory>
#include <vector>

class BrawlState : public State {
    using Brawlers = std::vector<std::unique_ptr<Brawler>>;

  public:
    BrawlState();

    void update() override;
    void draw() override;

  private:
    Brawlers m_brawlers;
    Arena m_arena;
};
