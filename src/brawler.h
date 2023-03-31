#pragma once
#include "raylib-cpp.hpp"

class Brawler {
  public:
    Brawler(Rectangle pos, int numJumps, int weight);

    virtual void draw();
    virtual void update();

  protected:
    int m_numJumps = 1;
    int m_weight = 10;
    int m_speed = 5;

    // state
    bool m_canMove = false;
    int m_damage = 0;
    bool m_isInAir = false;
    bool m_isBlocking = false;
    bool m_isPerformingAttack = false;
    int m_currentAttackFrame = 0;
    int m_currentJump = 0;

    Rectangle m_pos = {0, 0, 0, 0};
};
