#pragma once

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
};

enum class BrawlerAttacks {
    Basic,
    Special,
    Finisher,
};
