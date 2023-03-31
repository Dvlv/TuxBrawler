#include "brawlstate.h"
#include "constants.h"
#include "raylib-cpp.hpp"
#include "raylib.h"
#include <memory>

using UnqBrawlSt = std::unique_ptr<BrawlState>;

void update(UnqBrawlSt &bs) { bs->update(); }

void draw(UnqBrawlSt &bs) {
    BeginDrawing();

    ClearBackground(RAYWHITE);

    bs->draw();

    EndDrawing();
}

int main() {
    raylib::Window window(WINDOWWIDTH, WINDOWHEIGHT, "Tux Brawler");

    SetTargetFPS(FPS);

    UnqBrawlSt bs = std::make_unique<BrawlState>();

    while (!window.ShouldClose()) {
        update(bs);
        draw(bs);
    }

    return 0;
}
