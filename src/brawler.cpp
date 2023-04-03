#include "brawler.h"
#include "raylib-cpp.hpp"
#include "raylib.h"
#include <string_view>
#include <vector>

Brawler::Brawler(Vector2 pos, int numJumps, int weight, std::string name) {
    m_pos = pos;
    m_numJumps = numJumps;
    m_weight = weight;
    m_name = name;
}

Brawler::Brawler(json brawlerJson) {
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

    // TODO hitboxes
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
