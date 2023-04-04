#include "player.h"
#include "brawler.h"
#include "constants.h"
#include "raylib-cpp.hpp"
#include "raylib.h"
#include <algorithm>
#include <filesystem>

Player::Player(Vector2 pos, int numJumps, int weight, std::string name)
    : Brawler(pos, numJumps, weight, name) {}

Player::Player(json brawlerJson, std::filesystem::path jsonPath)
    : Brawler(brawlerJson, jsonPath) {}

void Player::draw() {
    if (m_isPerformingAttack) {
        DrawRectangleRec(Rectangle{m_pos.x, m_pos.y, 50, 50}, RED);

        if (m_attackBeingPerformed != nullptr) {
            std::string txt =
                "Atk: hs=" +
                std::to_string(m_attackBeingPerformed->hitStunTime) +
                " performed";

            DrawText(txt.c_str(), 400, 100, 40, BLACK);
        }
    } else {
        DrawRectangleRec(Rectangle{m_pos.x, m_pos.y, 50, 50}, GREEN);
    }
    DrawText(m_name.data(), m_pos.x, m_pos.y - 20, 20, BLACK);
}

void Player::update() {
    // Left / Right
    this->processMovementInputs();
    // this->processKnockback();

    // Jump / Gravity
    this->processJumpAndGravity();

    // Attack
    this->processAttackInputs();

    this->move();
}

void Player::processMovementInputs() {
    if (!m_canMove)
        return;

    bool isMoveButtonPressed = false;
    if (IsKeyDown(KEY_LEFT)) {
        isMoveButtonPressed = true;
        m_velocity.x -= m_speed;

    } else if (IsKeyDown(KEY_RIGHT)) {
        isMoveButtonPressed = true;
        m_velocity.x += m_speed;
    }

    if (!isMoveButtonPressed && m_velocity.x != 0) {
        if (m_velocity.x > 0) {
            m_velocity.x -= DRAG;
            if (m_velocity.x < 0) {
                m_velocity.x = 0;
            }
        } else {
            m_velocity.x += DRAG;
            if (m_velocity.x > 0) {
                m_velocity.x = 0;
            }
        }
    }
}

void Player::processJumpAndGravity() {
    if (IsKeyPressed(KEY_UP) && m_currentJump < m_numJumps && m_canMove) {
        m_velocity.y -= m_jumpStr;
        m_currentJump++;
        m_isInAir = true;

    } else {
        m_velocity.y += GRAVITY;
        // TODO solid ground
        if (m_pos.y >= 600) {
            m_pos.y = std::min(600, (int)m_pos.y);
            m_velocity.y = 0;

            // on floor, reset jump
            m_currentJump = 0;
            m_isInAir = false;
        }
    }
}

void Player::processAttackInputs() {
    if (!m_canMove)
        return;

    if (m_isPerformingAttack) {
        // already attacking, dont listen for next input until I have chaining
        // system
        m_currentAttackFrame++;
        if (m_currentAttackFrame >= 30) {
            m_isPerformingAttack = false;
            m_currentAttackFrame = 0;
            m_attackBeingPerformed = nullptr;
        }
        return;
    }
    if (IsKeyPressed(KEY_A)) {
        // basic attack
        m_isPerformingAttack = true;
        m_attackBeingPerformed = &m_attackData[BrawlerAttacks::Basic];

        // TODO choose attack based on some kind of graph?
    } else if (IsKeyPressed(KEY_S)) {
        // special
        m_isPerformingAttack = true;
        m_attackBeingPerformed = &m_attackData[BrawlerAttacks::Special];

    } else if (IsKeyPressed(KEY_D)) {
        // Finisher
        m_isPerformingAttack = true;
        m_attackBeingPerformed = &m_attackData[BrawlerAttacks::Finisher];
    }
}
