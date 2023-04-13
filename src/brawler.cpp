#include "brawler.h"
#include "Texture.hpp"
#include "brawlerjsonparser.h"
#include "constants.h"
#include "enums.h"
#include "raylib-cpp.hpp"
#include "raylib.h"
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <string_view>
#include <vector>

using CollisionRects = std::vector<Rectangle>;

Brawler::Brawler(Vector2 pos, int numJumps, int weight, std::string name) {
    m_pos = pos;
    m_numJumps = numJumps;
    m_weight = weight;
    m_name = name;
}

Brawler::Brawler(BrawlerData brawlerData) {
    m_pos = brawlerData.pos;
    m_numJumps = brawlerData.numJumps;
    m_weight = brawlerData.weight;
    m_speed = brawlerData.speed;
    m_name = brawlerData.name;
    m_charSelectSprite = raylib::Texture(brawlerData.charSelectSprite);

    m_animationData = brawlerData.animationData;
    m_attackData = brawlerData.attackData;
}

void Brawler::draw() {
#define DRAW_COLLISIONS false

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

void Brawler::update(CollisionRects &arenaCollisions) {
    this->processBlock();
    this->processLeftRightMovements();
    this->processAttack();
    this->processJumpAndGravity();

    this->move(arenaCollisions);
    this->animate();
}

void Brawler::move(CollisionRects &arenaCollisions) {
    // Cap
    // TODO knockback needs much higher cap
    // detect if in knockback, apply diff cap
    // TODO blocked by other players and walls
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

    // do movement first, then push back out if inside wall
    m_pos.x += m_velocity.x;
    m_pos.y += m_velocity.y;

    // flip sprites if moving backwards
    if (m_velocity.x < 0) {
        m_isFacingLeft = true;
    } else if (m_velocity.x > 0) {
        m_isFacingLeft = false;
    }

    // apply arena collisions
    bool is_moving_left = m_velocity.x < 0;
    bool is_moving_right = m_velocity.x > 0;
    bool is_moving_up = m_velocity.y < 0;
    bool is_moving_down = m_velocity.y > 0;

    Vector2 spriteBox = this->spritePos();
    Rectangle brawlerHitbox = {spriteBox.x, spriteBox.y, BRAWLER_WIDTH,
                               BRAWLER_HEIGHT};

    int i = 0;
    for (auto &surface : arenaCollisions) {
        ++i;
        if (CheckCollisionRecs(brawlerHitbox, surface)) {
            Rectangle collRec = GetCollisionRec(brawlerHitbox, surface);

            // check where collision rec is relative to me
            bool colliding_left = collRec.x <= brawlerHitbox.x &&
                                  collRec.width < brawlerHitbox.width;
            bool colliding_right = collRec.x > brawlerHitbox.x;
            bool colliding_up = collRec.y <= brawlerHitbox.y &&
                                collRec.height < brawlerHitbox.height;
            bool colliding_down = collRec.y > brawlerHitbox.y;

            if (is_moving_down && colliding_down) {
                // on ground
                m_pos.y -= collRec.height;
                m_velocity.y = 0;
                m_isInAir = false;
                m_currentJump = 0;
            } else if (is_moving_up && colliding_up) {
                // bumped on ceiling
                m_pos.y += collRec.height;
                m_velocity.y = 0;
            }

            if (is_moving_left && colliding_left) {
                m_pos.x += collRec.width;
                m_velocity.x = 0;
            } else if (is_moving_right && colliding_right) {
                m_pos.x -= collRec.width;
                m_velocity.x = 0;
            }
        }
    }
}

void Brawler::setMovable() { m_canMove = true; }

void Brawler::setPos(Vector2 pos) { m_pos = pos; }

raylib::Texture &Brawler::getCharSelectSprite() { return m_charSelectSprite; }

void Brawler::loadSpritesheets() {
    for (auto [anim, animData] : m_animationData) {
        if (animData.spriteSheet != "" &&
            std::filesystem::exists(animData.spriteSheet)) {
            m_animSpritesheets[anim] = raylib::Texture(animData.spriteSheet);
        }
    }
}

Vector2 Brawler::feetPos() {
    // brawler sprites are 100x75, brawler sits at the bottom-center
    // as 50x50, feet are at bottom-center of THAT
    return {m_pos.x + BRAWLER_SPRITE_WIDTH / 2,
            m_pos.y + BRAWLER_SPRITE_HEIGHT};
}

Vector2 Brawler::spritePos() {
    // brawler sprites are 100x75, brawler sits at the bottom-center
    // as 50x50
    return {m_pos.x + ((BRAWLER_SPRITE_WIDTH - BRAWLER_WIDTH) / 2),
            m_pos.y + (BRAWLER_SPRITE_HEIGHT - BRAWLER_HEIGHT)};
}

Vector2 Brawler::spriteOrigin() {
    Vector2 myArea = this->spritePos();
    Vector2 myAreaOrigin = {myArea.x + (BRAWLER_WIDTH / 2),
                            myArea.y + (BRAWLER_HEIGHT / 2)};

    return myAreaOrigin;
}

void Brawler::setAnimation(BrawlerAnimations anim) {
    if (anim == m_currentAnim)
        return;

    m_currentAnim = anim;
    m_currentAnimFrame = 0;
    m_animFrameTimer = 0;
}

void Brawler::animate() {
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

void Brawler::onAttackFinish() {
    m_isPerformingAttack = false;
    m_currentAttackFrame = 0;
    m_attackBeingPerformed = nullptr;
    this->setAnimation(BrawlerAnimations::Idle);
}

void Brawler::performAttack(BrawlerAttacks atk) {
    m_isPerformingAttack = true;
    m_attackBeingPerformed = &m_attackData[atk];
    this->setAnimation(m_attackAnimations.at(atk));
}

Rectangle Brawler::getActiveAttackHitbox() {
    if (m_attackBeingPerformed == nullptr) {
        return {0, 0, 0, 0};
    }

    Rectangle relativeAtkArea =
        m_attackBeingPerformed->hitboxes.at(m_currentAttackFrame);

    Vector2 myAreaOrigin = this->spriteOrigin();

    Rectangle atkHitbox = {myAreaOrigin.x + relativeAtkArea.x,
                           myAreaOrigin.y + relativeAtkArea.y,
                           relativeAtkArea.width, relativeAtkArea.height};
    if (m_isFacingLeft) {
        atkHitbox.x =
            myAreaOrigin.x - relativeAtkArea.x - relativeAtkArea.width;
    }

    return atkHitbox;
}

void Brawler::attackFrameUpdate() {
    if (m_attackBeingPerformed == nullptr)
        return;

    ++m_currentAttackFrame;

    if (m_currentAttackFrame >= m_attackBeingPerformed->animData.numFrames) {
        this->onAttackFinish();
    }
}

void Brawler::processLeftRightMovements() {
    if (!m_canMove)
        return;

    bool shouldMoveLeft = false; // TODO
    bool shouldMoveRight = false;

    bool isMoving = false;

    if (!m_isBlocking && !m_isPerformingAttack) {
        if (shouldMoveLeft) {
            isMoving = true;
            m_velocity.x -= m_speed;

            if (!m_isInAir && !m_isHitStun) {
                this->setAnimation(BrawlerAnimations::Run);
            }

        } else if (shouldMoveRight) {
            isMoving = true;
            m_velocity.x += m_speed;

            if (!m_isInAir && !m_isHitStun) {
                this->setAnimation(BrawlerAnimations::Run);
            }
        }

        if (!isMoving && !m_isInAir && !m_isPerformingAttack) {
            this->setAnimation(BrawlerAnimations::Idle);
        }
    }

    if (!isMoving && m_velocity.x != 0) {
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

void Brawler::processJumpAndGravity() {
    int rnd = rand();
    bool shouldJump = rnd % 9 == 0 && rnd % 12 == 0; // false; // TODO

    if (shouldJump && m_currentJump < m_numJumps && m_canMove &&
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

void Brawler::processAttack() {}

void Brawler::processBlock() {
    bool shouldBlock = false; // TODO
    if (shouldBlock && !m_isInAir) {
        m_isBlocking = true;
        this->setAnimation(BrawlerAnimations::Block);
    } else {
        m_isBlocking = false;

        if (m_currentAnim == BrawlerAnimations::Block) {
            this->setAnimation(BrawlerAnimations::Idle);
        }
    }
}
