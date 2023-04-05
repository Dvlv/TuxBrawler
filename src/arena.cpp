#include "arena.h"
#include "raylib-cpp.hpp"
#include "raylib.h"
#include <filesystem>
#include <string>
#include <string_view>

using CollisionRects = std::vector<Rectangle>;

Arena::Arena() {
    // TODO default a map here
}

Arena::Arena(json mapJson, std::filesystem::path jsonPath) {
    std::string background = ""; // TODO default map bg

    if (mapJson.contains("sprite")) {
        background = mapJson["sprite"];
    }

    std::filesystem::path backgroundFilePath =
        jsonPath / std::filesystem::path(background);

    m_background = raylib::Texture(backgroundFilePath);

    if (mapJson.contains("collisions")) {
        std::vector<std::vector<int>> collisions =
            mapJson["collisions"].get<std::vector<std::vector<int>>>();

        // this is an array of arrays of 4 ints, I.E. an array of Rectangle args
        for (auto &rectVec : collisions) {
            if (rectVec.size() != 4) {
                printf("A rectangle in map json collisions has !=4 ints, "
                       "ignoring!\n");
            } else {
                float x = (float)rectVec[0];
                float y = (float)rectVec[1];
                float w = (float)rectVec[2];
                float h = (float)rectVec[3];
                m_solidCollisionRects.push_back(Rectangle{x, y, w, h});
            }
        }
    }
}

void Arena::draw() {
    DrawTexture(m_background, 0, 0, WHITE);
    for (auto &r : m_solidCollisionRects) {
        DrawRectangleRec(r, {200, 0, 0, 125});
    }
}

CollisionRects Arena::getCollisionRects() { return m_solidCollisionRects; }
