#include "brawler.h"
#include "raylib-cpp.hpp"
#include "raylib.h"
#include <string_view>

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
}

BrawlerAnimData Brawler::parseAnimJson(json animData) {
    std::string spriteSheetPath = animData["sprite"];
    raylib::Texture tex(spriteSheetPath);

    return BrawlerAnimData{
        .spriteSheet = spriteSheetPath,
        .numFrames = animData["numFrames"],
        .animFPS = animData["animFPS"],
    };
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
