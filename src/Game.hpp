#pragma once
#include <unistd.h>
#include "GameField.hpp"
#include "Player.hpp"

class Game {
private:
    Player player;
    GameField* field;
    unsigned gameLevel;
public:
    Game();
    Game(int width, int height, unsigned level);
    ~Game();
    void startGameLoop();
};
