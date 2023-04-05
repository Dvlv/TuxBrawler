#pragma once
#include "brawler.h"
#include "raylib-cpp.hpp"
#include <filesystem>
#include <nlohmann/json.hpp>
using json = nlohmann::json;
using CollisionRects = std::vector<Rectangle>;

class Player : public Brawler {
  public:
    Player(Vector2 pos, int numJumps, int weight, std::string name);
    Player(json brawlerJson, std::filesystem::path jsonPath);

    virtual void draw() override;
    virtual void update(CollisionRects &arenaCollisions) override;

  private:
    void processMovementInputs();
    void processJumpAndGravity();
    void processAttackInputs();
};
