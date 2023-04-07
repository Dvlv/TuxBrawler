#include "brawlstate.h"
#include "constants.h"
#include "raylib-cpp.hpp"
#include "raylib.h"
#include "state.h"
#include <memory>

using UnqState = std::unique_ptr<State>;

void update(UnqState &cs) { cs->update(); }

void draw(UnqState &cs) {
    BeginDrawing();

    ClearBackground(RAYWHITE);

    cs->draw();

    EndDrawing();
}

int main() {
    raylib::Window window(WINDOWWIDTH, WINDOWHEIGHT, "Tux Brawler");

    SetTargetFPS(FPS);

    UnqState bs = std::make_unique<BrawlState>();

    while (!window.ShouldClose()) {
        update(bs);
        draw(bs);
    }

    return 0;
}
