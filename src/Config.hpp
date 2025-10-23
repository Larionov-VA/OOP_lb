#pragma once
#include <vector>
#include <string>
struct GlobalGameConfig {
    static int fieldWidth;
    static int fieldHeight;
    static int gameLevel;
    static enum difficulties {
        EASY, MEDIUM, HARD
    } difficulty;
};

extern std::vector<wchar_t> fieldChars;