#pragma once

#include "brawler.h"
#include "brawlerjsonparser.h"
#include "state.h"
#include <memory>
#include <vector>

class CharSelectState : public State {
    using SharedBrawler = std::shared_ptr<Brawler>;

  public:
    CharSelectState();

    void update() override;
    void draw() override;

    BrawlerData getSelectedBrawler();
    BrawlerData getRandomBrawler();

  private:
    std::vector<BrawlerData> m_brawlers{};
    std::vector<raylib::Texture> m_brawlerSprites{};

    BrawlerData m_selectedBrawler;
    int m_selectedBrawlerIdx = 0;
    Color m_selectBorderColor = {0, 255, 0, 255};
    int m_selectBorderAlphaModifier = 1;
};
