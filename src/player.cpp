#include "player.h"
#include "brawler.h"
#include "brawlerjsonparser.h"
#include "constants.h"
#include "raylib-cpp.hpp"
#include "raylib.h"
#include <algorithm>
#include <filesystem>

using CollisionRects = std::vector<Rectangle>;

Player::Player(Vector2 pos, int numJumps, int weight, std::string name)
    : Brawler(pos, numJumps, weight, name) {}

Player::Player(BrawlerData brawlerData) : Brawler(brawlerData) {}

void Player::draw() {
    if (m_isPerformingAttack) {
        DrawRectangleRec(
            Rectangle{m_pos.x, m_pos.y, BRAWLER_WIDTH, BRAWLER_HEIGHT}, RED);

        if (m_attackBeingPerformed != nullptr) {
            std::string txt =
                "Atk: hs=" +
                std::to_string(m_attackBeingPerformed->hitStunTime) +
                " performed";

            DrawText(txt.c_str(), 400, 100, 40, BLACK);
        }
    } else {
        // draw entire box
        DrawRectangleRec(Rectangle{m_pos.x, m_pos.y, BRAWLER_SPRITE_WIDTH,
                                   BRAWLER_SPRITE_HEIGHT},
                         GREEN);
        // draw feet pos
        DrawEllipse(this->feetPos().x, this->feetPos().y, 3, 3, RED);

        // draw sprite area
        DrawRectangle(this->spritePos().x, this->spritePos().y, BRAWLER_WIDTH,
                      BRAWLER_HEIGHT, BLUE);

        // draw actual texture
        Rectangle texRec;

        if (m_isFacingLeft) {
            texRec = {(float)(m_currentAnimFrame + 1) * BRAWLER_SPRITE_WIDTH, 0,
                      -BRAWLER_SPRITE_WIDTH, BRAWLER_SPRITE_HEIGHT};
        } else {
            texRec = {(float)m_currentAnimFrame * BRAWLER_SPRITE_WIDTH, 0,
                      BRAWLER_SPRITE_WIDTH, BRAWLER_SPRITE_HEIGHT};
        }

        DrawTextureRec(m_animSpritesheets[m_currentAnim], texRec, m_pos, WHITE);
    }
    DrawText(m_name.data(), m_pos.x, m_pos.y - 20, 20, BLACK);
}

void Player::update(CollisionRects &arenaCollisions) {
    // Left / Right
    this->processMovementInputs();
    // this->processKnockback();

    // Jump / Gravity
    this->processJumpAndGravity();

    // Attack
    this->processAttackInputs();

    this->move(arenaCollisions);
    this->animate();
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

void Player::animate() {
    ++m_animFrameTimer;
    // TODO store this in aniimData struct
    int frameChangePoint = 60 / m_animationData[m_currentAnim].animFPS;
    if (m_animFrameTimer >= frameChangePoint) {
        ++m_currentAnimFrame;
        m_animFrameTimer = 0;
        // currentAnimFrame is 0-indexed
        if (m_currentAnimFrame >= m_animationData[m_currentAnim].numFrames) {
            m_currentAnimFrame = 0;
        }
    }
}
