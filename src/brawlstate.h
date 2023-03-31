#pragma once
#include "arena.h"
#include "brawler.h"
#include "raylib-cpp.hpp"
#include <memory>
#include <vector>

class BrawlState {
    using Brawlers = std::vector<std::unique_ptr<Brawler>>;

  public:
    BrawlState();

    bool isFinshed();

    void draw();
    void update();

  private:
    Brawlers m_brawlers;
    bool m_isFinished = false;
    Arena m_arena;
};
