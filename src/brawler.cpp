#include "brawler.h"
#include "brawlerjsonparser.h"
#include "constants.h"
#include "raylib-cpp.hpp"
#include "raylib.h"
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
    DrawRectangleRec(Rectangle{m_pos.x, m_pos.y, 50, 50}, GREEN);
    DrawText(m_name.data(), m_pos.x, m_pos.y - 20, 20, BLACK);
}

void Brawler::update(CollisionRects &arenaCollisions) {
    this->move(arenaCollisions);
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

    // apply arena collisions
    bool is_moving_left = m_velocity.x < 0;
    bool is_moving_right = m_velocity.x > 0;
    bool is_moving_up = m_velocity.y < 0;
    bool is_moving_down = m_velocity.y > 0;

    Rectangle brawlerHitbox = {m_pos.x, m_pos.y, BRAWLER_WIDTH, BRAWLER_HEIGHT};

    int i = 0;
    for (auto &surface : arenaCollisions) {
        ++i;
        if (CheckCollisionRecs(brawlerHitbox, surface)) {
            Rectangle collRec = GetCollisionRec(brawlerHitbox, surface);

            // printf("Colliding %d: %f, %f, %f, %f\n", i, collRec.x, collRec.y,
            // brawlerHitbox.x, brawlerHitbox.y);

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
