#include "brawler.h"
#include "raylib-cpp.hpp"
#include "raylib.h"

Brawler::Brawler(Vector2 pos, int numJumps, int weight) {
    m_pos = pos;
    m_numJumps = numJumps;
    m_weight = weight;
}

void Brawler::draw() {
    DrawRectangleRec(Rectangle{m_pos.x, m_pos.y, 50, 50}, GREEN);
}

void Brawler::update() { this->move(); }

void Brawler::move() {
    // Cap
    // TODO knockback needs much higher cap
    // detect if in knockback, apply diff cap
    if (m_velocity.x > m_xSpeedCap) {
        m_velocity.x = m_xSpeedCap;

    } else if (m_velocity.x < -m_xSpeedCap) {
        m_velocity.x = -m_xSpeedCap;
    }

    if (m_velocity.y > m_ySpeedCap) {
        m_velocity.y = m_ySpeedCap;

    } else if (m_velocity.y < -m_ySpeedCap) {
        m_velocity.y = -m_ySpeedCap;
    }

    m_pos.x += m_velocity.x;
    m_pos.y += m_velocity.y;
}

void Brawler::setMovable() { m_canMove = true; }
