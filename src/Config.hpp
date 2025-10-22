#pragma once
#include <vector>

struct GlobalGameConfig {
    static int fieldWidth;
    static int fieldHeight;
    static int gameLevel;
    static enum difficulties {
        EASY, MEDIUM, HARD
    } difficulty;
};

extern std::vector<char> fieldChars;