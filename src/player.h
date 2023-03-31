#include "brawler.h"
#include "raylib-cpp.hpp"

class Player : public Brawler {
  public:
    Player(Rectangle pos, int numJumps, int weight);

    virtual void draw() override;
    virtual void update() override;
};
