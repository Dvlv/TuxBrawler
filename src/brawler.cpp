#include "brawler.h"
#include "raylib-cpp.hpp"
#include "raylib.h"

Brawler::Brawler(Rectangle pos, int numJumps, int weight) {
    m_pos = pos;
    m_numJumps = numJumps;
    m_weight = weight;
}

void Brawler::draw() { DrawRectangleRec(m_pos, GREEN); }

void Brawler::update() {
    // This is the AI of the CPU characters
    m_pos.x += m_speed;
    m_pos.y += m_speed;
}
