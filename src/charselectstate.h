#pragma once

#include "brawler.h"
#include "state.h"
#include <memory>
#include <vector>

class CharSelectState : public State {
    using SharedBrawler = std::shared_ptr<Brawler>;

  public:
    CharSelectState();

    void update() override;
    void draw() override;

    SharedBrawler getSelectedBrawler();

  private:
    std::vector<SharedBrawler> m_brawlers{};
    SharedBrawler m_selectedBrawler;
    int m_selectedBrawlerIdx = 0;
    Color m_selectBorderColor = {0, 255, 0, 255};
    int m_selectBorderAlphaModifier = 1;
};
