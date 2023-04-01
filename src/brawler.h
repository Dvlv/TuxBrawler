#pragma once
#include "raylib-cpp.hpp"

class Brawler {
  public:
    Brawler(Vector2 pos, int numJumps, int weight);

    virtual void draw();
    virtual void update();
    virtual void move();

    void setMovable();

  protected:
    int m_numJumps = 1;
    int m_weight = 10;
    int m_speed = 3;
    int m_xSpeedCap = 9;
    int m_ySpeedCap = 45;
    int m_jumpStr = 22;

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
