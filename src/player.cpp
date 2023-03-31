#include "player.h"
#include "brawler.h"
#include "raylib-cpp.hpp"
#include "raylib.h"

Player::Player(Rectangle pos, int numJumps, int weight)
    : Brawler(pos, numJumps, weight) {}

void Player::draw() { DrawRectangleRec(m_pos, GREEN); }

void Player::update() {
    if (IsKeyDown(KEY_LEFT)) {
        m_pos.x -= m_speed;
    } else if (IsKeyDown(KEY_RIGHT)) {
        m_pos.x += m_speed;
    }
}
