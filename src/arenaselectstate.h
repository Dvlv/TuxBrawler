#pragma once

#include "arena.h"
#include "state.h"
#include <memory>

class ArenaSelectState : public State {
  public:
    ArenaSelectState();

    void update() override;
    void draw() override;

    std::shared_ptr<Arena> getSelectedArena();

  private:
    std::vector<std::shared_ptr<Arena>> m_arenas{};
    std::vector<raylib::Texture> m_arenaSprites{};

    std::shared_ptr<Arena> m_selectedArena;
    int m_selectedArenaIdx = 0;
    Color m_selectBorderColor = {0, 255, 0, 255};
    int m_selectBorderAlphaModifier = 1;
};
