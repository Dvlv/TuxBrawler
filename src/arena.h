#pragma once
#include "raylib-cpp.hpp"
#include <filesystem>
#include <nlohmann/json.hpp>
#include <string>
#include <string_view>
#include <vector>

class Arena {
    using CollisionRects = std::vector<Rectangle>;
    using json = nlohmann::json;

  public:
    Arena(); // TODO does this actually trick the compiler?
    Arena(json mapJson, std::filesystem::path jsonPath);

    // maps won't update to start with
    void draw();

  private:
    raylib::Texture2D m_background;
    CollisionRects m_solidCollisionRects = std::vector<Rectangle>{};
};
