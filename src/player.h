#include "brawler.h"
#include "raylib-cpp.hpp"
#include <nlohmann/json.hpp>
using json = nlohmann::json;

class Player : public Brawler {
  public:
    Player(Vector2 pos, int numJumps, int weight, std::string name);
    Player(json brawlerJson);

    virtual void draw() override;
    virtual void update() override;

  private:
    void processMovementInputs();
    void processJumpAndGravity();
    void processAttackInputs();
};
