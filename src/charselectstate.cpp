#include "charselectstate.h"
#include "Functions.hpp"
#include "constants.h"
#include "raylib-cpp.hpp"
#include "raylib.h"
#include <array>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <nlohmann/json.hpp>

using SharedBrawler = std::shared_ptr<Brawler>;

CharSelectState::CharSelectState() {
    using path = std::filesystem::path;

    std::vector<path> brawlerFolders{};

    const path charFolder = "src/resources/brawlers";
    for (const auto &childDir :
         std::filesystem::directory_iterator(charFolder)) {
        if (std::filesystem::is_directory(childDir)) {
            brawlerFolders.push_back(childDir);
        }
    }

    for (auto &brawlerFolder : brawlerFolders) {
        const path jsonPath = brawlerFolder / "brawler.json";
        std::cout << jsonPath << std::endl;
        if (std::filesystem::exists(jsonPath)) {

            std::cout << brawlerFolder << std::endl;
            json brawlerJson;
            std::ifstream brawlerFile(jsonPath);
            brawlerFile >> brawlerJson;

            m_brawlers.push_back(
                std::make_shared<Brawler>(brawlerJson, brawlerFolder));
        }
    }
}

void CharSelectState::update() {
    // listen for Enter to select and return char
    if (IsKeyPressed(KEY_ENTER)) {
        m_selectedBrawler = m_brawlers.at(m_selectedBrawlerIdx);
        m_isFinished = true;

        return;
    }

    // listen for keypress and move selected brawler
    bool wasSelectionChanged = false;
    if (IsKeyPressed(KEY_RIGHT)) {
        if (m_selectedBrawlerIdx < (m_brawlers.size() - 1)) {
            m_selectedBrawlerIdx += 1;
            wasSelectionChanged = true;
        }
    } else if (IsKeyPressed(KEY_LEFT)) {
        if (m_selectedBrawlerIdx > 0) {
            m_selectedBrawlerIdx -= 1;
            wasSelectionChanged = true;
        }
    }

    // flashing of selected char border
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
void CharSelectState::draw() {
    ClearBackground(SKYBLUE);

    // Draw Title
    std::string titleText = "Choose Your Brawler";
    int textWidth = raylib::MeasureText(titleText, TITLE_FONT_SIZE);
    DrawText(titleText.c_str(), WINDOWWIDTH / 2 - textWidth / 2,
             TITLE_FONT_SIZE, TITLE_FONT_SIZE, BLACK);

    // Draw all characters
    const int MAX_PER_ROW = 5;
    const std::array<int, 5> XES = {150, 350, 550, 750, 950};
    const int STARTINGY = 200;
    const int SELECT_BORDER_WITH = 10;

    Vector2 nextPos = {
        (float)XES[0],
        STARTINGY,
    };

    int idx = 0;
    int brawlerIdx = 0;
    for (auto &brawler : m_brawlers) {
        if (brawlerIdx == m_selectedBrawlerIdx) {
            DrawRectangle(nextPos.x - SELECT_BORDER_WITH,
                          nextPos.y - SELECT_BORDER_WITH,
                          CSSPRITE_WIDTH + (SELECT_BORDER_WITH * 2),
                          CSSPRITE_HEIGHT + (SELECT_BORDER_WITH * 2),
                          m_selectBorderColor);
        }

        DrawTextureV(brawler->getCharSelectSprite(), nextPos, WHITE);

        brawlerIdx++;
        ++idx;
        if (idx > 4) {
            idx = 0;
            nextPos.y += STARTINGY;
        }
        nextPos.x = XES[idx];
    }
}

SharedBrawler CharSelectState::getSelectedBrawler() {
    return m_selectedBrawler;
}
