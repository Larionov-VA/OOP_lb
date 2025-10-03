#pragma once
#include "GameField.hpp"
#include "Player.hpp"

class Game {
private:
    Player player;
    GameField* field;
    unsigned gameLevel;
public:
    Game();
    ~Game();
    void start();
};
