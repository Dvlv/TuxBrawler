#pragma once
#include "raylib-cpp.hpp"
#include <string>
#include <string_view>
#include <vector>

class Arena {
    using CollisionRects = std::vector<Rectangle>;

  public:
    Arena(std::string_view mapFilePath);

    // maps won't update to start with
    void draw();

  private:
    raylib::Texture2D m_background;
    CollisionRects m_solidCollisionRects;
    CollisionRects m_oneWayCollisionRects;
};
