#include "arenaselectstate.h"
#include "brawlstate.h"
#include "charselectstate.h"
#include "constants.h"
#include "raylib-cpp.hpp"
#include "raylib.h"
#include "state.h"
#include <memory>

using UnqState = std::unique_ptr<State>;
using SharedBrawler = std::shared_ptr<Brawler>;

// TODO not global
static GameState gs = GameState::ArenaSelect;
static std::unique_ptr<BrawlState> bs;
static std::unique_ptr<CharSelectState> css;
static std::unique_ptr<ArenaSelectState> ass;

static State *cs;

void update() {
    cs->update();

    if (cs->isFinshed()) {
        // state finished, move to next
        switch (gs) {
        case GameState::CharSelect:
            gs = GameState::Brawl;

            bs->setPlayerBrawler(css->getSelectedBrawler());
            bs->addCpuBrawler(css->getRandomBrawler());

            cs = bs.get();

            break;
        case GameState::Brawl:
            gs = GameState::CharSelect;
            break;
        case GameState::ArenaSelect:
            gs = GameState::CharSelect;
            bs->setArena(ass->getSelectedArena());

            cs = css.get();
            break;
        default:
            gs = GameState::CharSelect;
        }
    }
}

void draw() {
    BeginDrawing();

    ClearBackground(RAYWHITE);

    cs->draw();

    EndDrawing();
}

int main() {
    raylib::Window window(WINDOWWIDTH, WINDOWHEIGHT, "Tux Brawler");

    SetTargetFPS(FPS);

    bs = std::make_unique<BrawlState>();
    css = std::make_unique<CharSelectState>();
    ass = std::make_unique<ArenaSelectState>();

    cs = ass.get();

    while (!window.ShouldClose()) {
        update();
        draw();
    }

    return 0;
}
