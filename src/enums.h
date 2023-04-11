#pragma once
#include <unordered_map>

enum class GameState {
    Title,
    CharSelect,
    Menu,
    Brawl,
};

enum class BrawlerAnimations {
    Idle,
    Run,
    Block,
    Hit,
    Jump,
    Attack_Basic,
    Attack_Special,
    Attack_Finisher,
};

enum class BrawlerAttacks {
    Basic,
    Special,
    Finisher,
};
