#include "brawler.h"
#include "raylib-cpp.hpp"

class Player : public Brawler {
  public:
    Player(Vector2 pos, int numJumps, int weight);

    virtual void draw() override;
    virtual void update() override;

  private:
    void processMovementInputs();
    void processJumpAndGravity();
    void processAttackInputs();
};
