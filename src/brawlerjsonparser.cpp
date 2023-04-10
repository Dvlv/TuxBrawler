#include "brawlerjsonparser.h"
#include "enums.h"
#include <iostream>

BrawlerData
BrawlerJsonParser::parseBrawlerJson(json brawlerJson,
                                    std::filesystem::path jsonPath) {
    // TODO proper defaults for these
    std::filesystem::path spritesPath = jsonPath / "sprites";

    BrawlerData data{
        .pos = Vector2{0, 0},
        .numJumps = brawlerJson["numJumps"],
        .weight = brawlerJson["weight"],
        .speed = brawlerJson["speed"],
        .name = (std::string)brawlerJson["name"],
        .charSelectSprite = spritesPath / brawlerJson["charSelectSprite"],
    };

    for (auto anim : brawlerJson["animations"].items()) {
        std::string animName = anim.key();

        if (animName == "idle") {
            data.animationData[BrawlerAnimations::Idle] =
                this->parseAnimJson(anim.value(), jsonPath);
        } else if (animName == "run") {

            data.animationData[BrawlerAnimations::Run] =
                this->parseAnimJson(anim.value(), jsonPath);
        } else if (animName == "block") {
            data.animationData[BrawlerAnimations::Block] =
                this->parseAnimJson(anim.value(), jsonPath);

        } else if (animName == "hit") {
            data.animationData[BrawlerAnimations::Hit] =
                this->parseAnimJson(anim.value(), jsonPath);

        } else if (animName == "jump") {
            data.animationData[BrawlerAnimations::Jump] =
                this->parseAnimJson(anim.value(), jsonPath);
        }
    }

    for (auto atk : brawlerJson["attacks"].items()) {
        std::string atkType = atk.key();

        if (atkType == "basic") {
            data.attackData[BrawlerAttacks::Basic] =
                this->parseAtkJson(atk.value(), jsonPath);

            data.animationData[BrawlerAnimations::Attack_Basic] =
                data.attackData[BrawlerAttacks::Basic].animData;
        } else if (atkType == "special") {
            data.attackData[BrawlerAttacks::Special] =
                this->parseAtkJson(atk.value(), jsonPath);

            data.animationData[BrawlerAnimations::Attack_Special] =
                data.attackData[BrawlerAttacks::Special].animData;
        } else if (atkType == "finisher") {
            data.attackData[BrawlerAttacks::Finisher] =
                this->parseAtkJson(atk.value(), jsonPath);

            data.animationData[BrawlerAnimations::Attack_Finisher] =
                data.attackData[BrawlerAttacks::Finisher].animData;
        }
    }

    return data;
}

BrawlerAnimData
BrawlerJsonParser::parseAnimJson(json animData,
                                 std::filesystem::path jsonPath) {
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
        .spriteSheet = jsonPath / "sprites" / sprite,
        .numFrames = numFrames,
        .animFPS = animFPS,
    };
}

BrawlerAttackData
BrawlerJsonParser::parseAtkJson(json atkData, std::filesystem::path jsonPath) {
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

    return BrawlerAttackData{
        .animData =
            BrawlerAnimData{.spriteSheet = jsonPath / "sprites" / sprite,
                            .numFrames = numFrames,
                            .animFPS = animFPS},
        .hitStunTime = hitStunTime,
        .hitboxes = hitboxRects};
}
