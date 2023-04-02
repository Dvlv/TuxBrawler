#pragma once
#include "enums.h"
#include "raylib-cpp.hpp"
#include <map>
#include <nlohmann/json.hpp>
#include <string_view>
#include <unordered_map>
#include <vector>

using json = nlohmann::json;
using TexVec = std::vector<raylib::Texture>;

typedef struct {
    raylib::Texture spriteSheet;
    int numFrames;
    int animFPS;
} BrawlerAnimData;

typedef struct {
    raylib::Texture spriteSheet;
    int numFrames;
    int animFPS;
    double hitStunTime;
    std::vector<Rectangle> hitboxes;
} BrawlerAttackData;

class Brawler {
  public:
    Brawler(Vector2 pos, int numJumps, int weight, std::string name);
    Brawler(json brawlerJson);

    virtual void draw();
    virtual void update();
    virtual void move();

    BrawlerAnimData parseAnimJson(json animData);
    BrawlerAttackData parseAtkJson(json atkData);

    void setMovable();

  protected:
    std::string m_name;
    int m_numJumps = 1;
    int m_weight = 10;
    int m_speed = 3;
    int m_xSpeedCap = 9;
    int m_ySpeedCap = 45;
    int m_jumpStr = 22;

    raylib::Texture m_charSelectSprite;
    std::unordered_map<BrawlerAnimations, BrawlerAnimData> m_animationData;
    std::unordered_map<BrawlerAttacks, BrawlerAttackData> m_attackData;

    // state
    bool m_canMove = false;
    bool m_isInAir = false;
    bool m_isBlocking = false;
    bool m_isPerformingAttack = false;
    bool m_isInKnockback = false;
    bool m_isHitStun = false;

    int m_damage = 0;
    int m_currentAttackFrame = 0;
    int m_currentJump = 0;
    Vector2 m_velocity = Vector2{.x = 0, .y = 0};

    Vector2 m_pos = {0, 0};
};
