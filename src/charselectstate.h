#pragma once

#include "brawler.h"
#include "state.h"
#include <memory>
#include <vector>

class CharSelectState : public State {
    using SharedBrawler = std::shared_ptr<Brawler>;

  public:
    CharSelectState();

  private:
    std::vector<SharedBrawler> m_brawlers{};
    SharedBrawler m_selectedBrawler;
};
