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
#define DRAW_COLLISIONS true

    if (m_isPerformingAttack) {
#if DRAW_COLLISIONS
        DrawRectangleRec(
            Rectangle{m_pos.x, m_pos.y, BRAWLER_WIDTH, BRAWLER_HEIGHT}, RED);
#endif

        if (m_attackBeingPerformed != nullptr) {
            std::string txt =
                "Atk: hs=" +
                std::to_string(m_attackBeingPerformed->hitStunTime) +
                " performed";

            DrawText(txt.c_str(), 400, 100, 40, BLACK);
        }
    }
#if DRAW_COLLISIONS
    // draw entire box
    DrawRectangleRec(Rectangle{m_pos.x, m_pos.y, BRAWLER_SPRITE_WIDTH,
                               BRAWLER_SPRITE_HEIGHT},
                     GREEN);
    // draw feet pos
    DrawEllipse(this->feetPos().x, this->feetPos().y, 3, 3, RED);

    // draw sprite area
    DrawRectangle(this->spritePos().x, this->spritePos().y, BRAWLER_WIDTH,
                  BRAWLER_HEIGHT, BLUE);

    // draw attack hitbox
    if (m_isPerformingAttack && m_attackBeingPerformed != nullptr) {
        DrawRectangleRec(this->getActiveAttackHitbox(), {125, 125, 0, 120});
    }

#endif
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
    // DrawTexturePro(m_animSpritesheets[m_currentAnim], texRec,
    //{m_pos.x - (BRAWLER_SPRITE_WIDTH / 2),
    // m_pos.y - BRAWLER_SPRITE_HEIGHT,
    // BRAWLER_SPRITE_WIDTH * 2, BRAWLER_SPRITE_HEIGHT * 2},
    //{0, 0}, 0, WHITE);

    // DrawText(m_name.data(), m_pos.x, m_pos.y - 20, 20, BLACK);
}

void Player::update(CollisionRects &arenaCollisions) {
    // Block
    this->processBlockInput();

    // Left / Right
    this->processMovementInputs();
    // Attack
    this->processAttackInputs();

    // Jump / Gravity
    this->processJumpAndGravity();

    // this->processKnockback();
    this->move(arenaCollisions);
    this->animate();
}

void Player::processMovementInputs() {
    if (!m_canMove)
        return;

    bool isMoveButtonPressed = false;

    if (!m_isBlocking && !m_isPerformingAttack) {
        if (IsKeyDown(KEY_LEFT)) {
            isMoveButtonPressed = true;
            m_velocity.x -= m_speed;

            if (!m_isInAir && !m_isHitStun) {
                this->setAnimation(BrawlerAnimations::Run);
            }

        } else if (IsKeyDown(KEY_RIGHT)) {
            isMoveButtonPressed = true;
            m_velocity.x += m_speed;

            if (!m_isInAir && !m_isHitStun) {
                this->setAnimation(BrawlerAnimations::Run);
            }
        }

        if (!isMoveButtonPressed && !m_isInAir && !m_isPerformingAttack) {
            this->setAnimation(BrawlerAnimations::Idle);
        }
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
    if (IsKeyPressed(KEY_UP) && m_currentJump < m_numJumps && m_canMove &&
        !m_isBlocking) {
        m_velocity.y -= m_jumpStr;
        m_currentJump++;
        m_isInAir = true;

        if (!m_isHitStun && !m_isPerformingAttack) {
            this->setAnimation(BrawlerAnimations::Jump);
        }
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
        return;
    }
    if (IsKeyPressed(KEY_A)) {
        // basic attack
        this->performAttack(BrawlerAttacks::Basic);

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
    if (m_animationData[m_currentAnim].numFrames == 1) {
        // anim is a static frame, no need to animate
        return;
    }

    ++m_animFrameTimer;
    // TODO store this in aniimData struct
    int frameChangePoint = 60 / m_animationData[m_currentAnim].animFPS;
    if (m_animFrameTimer >= frameChangePoint) {
        ++m_currentAnimFrame;
        m_animFrameTimer = 0;

        if (m_isPerformingAttack) {
            this->attackFrameUpdate();
        }

        // currentAnimFrame is 0-indexed
        if (m_currentAnimFrame >= m_animationData[m_currentAnim].numFrames) {
            m_currentAnimFrame = 0;
        }
    }
}

void Player::processBlockInput() {
    if (IsKeyDown(KEY_Q) && !m_isInAir) {
        m_isBlocking = true;
        this->setAnimation(BrawlerAnimations::Block);
    } else {
        m_isBlocking = false;

        if (m_currentAnim == BrawlerAnimations::Block) {
            this->setAnimation(BrawlerAnimations::Idle);
        }
    }
}
