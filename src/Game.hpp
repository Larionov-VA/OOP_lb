#pragma once
#include "GameField.hpp"

class Game {
private:
    GameField* field;
    unsigned gameLevel;
public:
    Game();
    ~Game();
    void start();
};
