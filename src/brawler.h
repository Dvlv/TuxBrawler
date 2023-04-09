#pragma once
#include "brawlerjsonparser.h"
#include "enums.h"
#include "raylib-cpp.hpp"
#include <filesystem>
#include <map>
#include <nlohmann/json.hpp>
#include <string_view>
#include <unordered_map>
#include <vector>

using json = nlohmann::json;
using TexVec = std::vector<raylib::Texture>;
using CollisionRects = std::vector<Rectangle>;

class Brawler {
  public:
    Brawler(Vector2 pos, int numJumps, int weight, std::string name);
    Brawler(BrawlerData brawlerData);

    virtual void draw();
    virtual void update(CollisionRects &arenaCollisions);
    virtual void move(CollisionRects &arenaCollisions);
    virtual void animate();

    void setAnimation(BrawlerAnimations anim);
    void setMovable();
    void setPos(Vector2 pos);

    void loadSpritesheets();
    Vector2 feetPos();
    Vector2 spritePos();

    raylib::Texture &getCharSelectSprite();

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

    std::map<BrawlerAnimations, raylib::Texture> m_animSpritesheets;
    std::map<BrawlerAttacks, raylib::Texture> m_atkSpritesheets;

    // state
    bool m_canMove = false;
    bool m_isInAir = false;
    bool m_isBlocking = false;
    bool m_isInKnockback = false;
    bool m_isHitStun = false;
    bool m_isPerformingAttack = false;
    BrawlerAttackData *m_attackBeingPerformed = nullptr;
    BrawlerAnimations m_currentAnim = BrawlerAnimations::Idle;
    int m_currentAnimFrame = 0;
    int m_animFrameTimer = 0;

    int m_damage = 0;
    int m_currentAttackFrame = 0;
    int m_currentJump = 0;
    Vector2 m_velocity = Vector2{0, 0};
    Vector2 m_pos = {0, 0};
};
