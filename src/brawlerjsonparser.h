#pragma once

#include "enums.h"
#include "raylib-cpp.hpp"
#include <filesystem>
#include <nlohmann/json.hpp>
#include <vector>

using json = nlohmann::json;

typedef struct {
    std::filesystem::path spriteSheet;
    int numFrames;
    int animFPS;
} BrawlerAnimData;

typedef struct {
    BrawlerAnimData animData;
    double hitStunTime;
    std::vector<Rectangle> hitboxes;

} BrawlerAttackData;

typedef struct {
    Vector2 pos;
    int numJumps;
    int weight;
    int speed;
    std::string name;
    std::filesystem::path charSelectSprite;

    std::unordered_map<BrawlerAnimations, BrawlerAnimData> animationData;
    std::unordered_map<BrawlerAttacks, BrawlerAttackData> attackData;

} BrawlerData;

class BrawlerJsonParser {
  public:
    BrawlerJsonParser() = default;

    BrawlerData parseBrawlerJson(json brawlerJson,
                                 std::filesystem::path jsonPath);
    BrawlerAnimData parseAnimJson(json animData,
                                  std::filesystem::path jsonPath);
    BrawlerAttackData parseAtkJson(json atkData,
                                   std::filesystem::path jsonPath);
};
