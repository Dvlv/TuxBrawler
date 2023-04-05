#include "brawler.h"
#include "constants.h"
#include "raylib-cpp.hpp"
#include "raylib.h"
#include <string_view>
#include <vector>

using CollisionRects = std::vector<Rectangle>;

Brawler::Brawler(Vector2 pos, int numJumps, int weight, std::string name) {
    m_pos = pos;
    m_numJumps = numJumps;
    m_weight = weight;
    m_name = name;
}

Brawler::Brawler(json brawlerJson, std::filesystem::path jsonPath) {
    m_pos = Vector2{0, 0};
    m_numJumps = brawlerJson["numJumps"];
    m_weight = brawlerJson["weight"];
    m_speed = brawlerJson["speed"];
    m_name = (std::string)brawlerJson["name"];

    for (auto anim : brawlerJson["animations"].items()) {
        std::string animName = anim.key();

        if (animName == "idle") {
            m_animationData[BrawlerAnimations::Idle] =
                this->parseAnimJson(anim.value());
        } else if (animName == "run") {

            m_animationData[BrawlerAnimations::Run] =
                this->parseAnimJson(anim.value());
        } else if (animName == "block") {
            m_animationData[BrawlerAnimations::Block] =
                this->parseAnimJson(anim.value());

        } else if (animName == "hit") {
            m_animationData[BrawlerAnimations::Hit] =
                this->parseAnimJson(anim.value());

        } else if (animName == "jump") {
            m_animationData[BrawlerAnimations::Jump] =
                this->parseAnimJson(anim.value());
        }
    }

    for (auto atk : brawlerJson["attacks"].items()) {
        std::string atkType = atk.key();

        if (atkType == "basic") {
            m_attackData[BrawlerAttacks::Basic] =
                this->parseAtkJson(atk.value());
        } else if (atkType == "special") {
            m_attackData[BrawlerAttacks::Special] =
                this->parseAtkJson(atk.value());
        } else if (atkType == "finisher") {
            m_attackData[BrawlerAttacks::Finisher] =
                this->parseAtkJson(atk.value());
        }
    }
}

BrawlerAnimData Brawler::parseAnimJson(json animData) {
    // TODO should these presence-checks be negative, so I can do a "you are
    // missing XX" warning?
    std::string sprite = ""; // TODO default
    int numFrames = 1;
    int animFPS = 1;

    if (animData.contains("sprite")) {
        sprite = animData["sprite"];
    }

    if (animData.contains("numFrames")) {
        numFrames = animData["numFrames"];
    }

    if (animData.contains("animFPS")) {
        animFPS = animData["animFPS"];
    }

    return BrawlerAnimData{
        //.spriteSheet = raylib::Texture(sprite),
        .numFrames = numFrames,
        .animFPS = animFPS,
    };
}

BrawlerAttackData Brawler::parseAtkJson(json atkData) {
    std::string sprite = ""; // TODO default
    int numFrames = 1;
    int animFPS = 1;
    double hitStunTime = 0.5;

    if (atkData.contains("sprite")) {
        sprite = atkData["sprite"];
    }

    if (atkData.contains("numFrames")) {
        numFrames = atkData["numFrames"];
    }

    if (atkData.contains("animFPS")) {
        animFPS = atkData["animFPS"];
    }

    if (atkData.contains("hitStunTime")) {
        hitStunTime = atkData["hitStunTime"];
    }

    std::vector<Rectangle> hitboxRects{};

    if (atkData.contains("hitboxes")) {
        std::vector<std::vector<int>> hitboxes =
            atkData["hitboxes"].get<std::vector<std::vector<int>>>();

        // this is an array of arrays of 4 ints, I.E. an array of Rectangle args
        for (auto &rectVec : hitboxes) {
            if (rectVec.size() != 4) {
                printf("A rectangle in Attack json has !=4 ints, ignoring!\n");
            } else {
                float x = (float)rectVec[0];
                float y = (float)rectVec[1];
                float w = (float)rectVec[2];
                float h = (float)rectVec[3];
                hitboxRects.push_back(Rectangle{x, y, w, h});
            }
        }
    }

    return BrawlerAttackData{//.spriteSheet = raylib::Texture(sprite),
                             .numFrames = numFrames,
                             .animFPS = animFPS,
                             .hitStunTime = hitStunTime,
                             .hitboxes = hitboxRects};
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
