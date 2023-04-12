#include "arenaselectstate.h"
#include "arena.h"
#include "constants.h"
#include "raylib-cpp.hpp"
#include <fstream>
#include <iostream>
#include <memory>
#include <nlohmann/json.hpp>

ArenaSelectState::ArenaSelectState() {
    using path = std::filesystem::path;
    using json = nlohmann::json;

    std::vector<path> arenaFolders{};

    const path arFolder = "src/resources/arenas";
    for (const auto &childDir : std::filesystem::directory_iterator(arFolder)) {
        if (std::filesystem::is_directory(childDir)) {
            arenaFolders.push_back(childDir);
        }
    }

    for (auto &arenaFolder : arenaFolders) {
        const path jsonPath = arenaFolder / "arena.json";
        std::cout << jsonPath << std::endl;
        if (std::filesystem::exists(jsonPath)) {
            json arenaJson;
            std::cout << arenaFolder << std::endl;

            std::ifstream arenaFile(jsonPath);
            arenaFile >> arenaJson;

            auto arena = std::make_shared<Arena>(arenaJson, arenaFolder);

            m_arenas.push_back(arena);
            m_arenaSprites.push_back(
                raylib::Texture(arenaFolder / arena->getArenaSelectSprite()));
        }
    }
}

void ArenaSelectState::update() {
    // listen for Enter to select and return arena
    if (IsKeyPressed(KEY_ENTER)) {
        m_selectedArena = m_arenas.at(m_selectedArenaIdx);
        m_isFinished = true;

        return;
    }

    // listen for keypress and move selected arena
    bool wasSelectionChanged = false;
    if (IsKeyPressed(KEY_RIGHT)) {
        if (m_selectedArenaIdx < (m_arenas.size() - 1)) {
            m_selectedArenaIdx += 1;
            wasSelectionChanged = true;
        }
    } else if (IsKeyPressed(KEY_LEFT)) {
        if (m_selectedArenaIdx > 0) {
            m_selectedArenaIdx -= 1;
            wasSelectionChanged = true;
        }
    }

    // flashing of selected arena border
    if (wasSelectionChanged) {
        // keep border solid if it was moved
        m_selectBorderColor.a = 255;
    } else {
        // update alpha of select border
        m_selectBorderColor.a -= m_selectBorderAlphaModifier;
    }

    if (m_selectBorderColor.a == 100 && m_selectBorderAlphaModifier == 1) {
        m_selectBorderColor.a = 101;
        m_selectBorderAlphaModifier = -1;
    } else if (m_selectBorderColor.a == 255 &&
               m_selectBorderAlphaModifier == -1) {
        m_selectBorderColor.a = 254;
        m_selectBorderAlphaModifier = 1;
    }
}
void ArenaSelectState::draw() {
    ClearBackground(SKYBLUE);

    // Draw Title
    std::string titleText = "Choose Your Arena";
    int textWidth = raylib::MeasureText(titleText, TITLE_FONT_SIZE);
    DrawText(titleText.c_str(), WINDOWWIDTH / 2 - textWidth / 2,
             TITLE_FONT_SIZE, TITLE_FONT_SIZE, BLACK);

    // Draw all characters
    const int NAME_FONT_SIZE = 25;
    const int MAX_PER_ROW = 5;
    const std::array<int, 5> XES = {150, 350, 550, 750, 950};
    const int STARTINGY = 200;
    const int SELECT_BORDER_WIDTH = 10;

    Vector2 nextPos = {
        (float)XES[0],
        STARTINGY,
    };

    int idx = 0;
    int arenaIdx = 0;
    for (auto &arena : m_arenas) {
        if (arenaIdx == m_selectedArenaIdx) {
            DrawRectangle(nextPos.x - SELECT_BORDER_WIDTH,
                          nextPos.y - SELECT_BORDER_WIDTH,
                          CSSPRITE_WIDTH + (SELECT_BORDER_WIDTH * 2),
                          CSSPRITE_HEIGHT + (SELECT_BORDER_WIDTH * 2),
                          m_selectBorderColor);
        }

        // sprite
        DrawTextureV(m_arenaSprites.at(idx), nextPos, WHITE);
        // name
        int nameWidth = MeasureText(arena->m_name.c_str(), NAME_FONT_SIZE);
        int namePosX = nextPos.x + (CSSPRITE_WIDTH / 2) - (nameWidth / 2);

        DrawText(arena->m_name.c_str(), namePosX,
                 nextPos.y + CSSPRITE_HEIGHT + SELECT_BORDER_WIDTH, 25, BLACK);

        arenaIdx++;
        ++idx;
        if (idx > 4) {
            idx = 0;
            nextPos.y += STARTINGY;
        }
        nextPos.x = XES[idx];
    }
}

std::shared_ptr<Arena> ArenaSelectState::getSelectedArena() {
    return m_selectedArena;
}
