#pragma once
#include <unistd.h>
#include "GameField.hpp"
#include "Player.hpp"
#include "IGameController.hpp"
#include "Config.hpp"

class Game : public IGameController{
private:
    Player player;
    GameField* field;
public:
    Game();
    ~Game();
    void startGame() override;
    bool performAnAction(char playerAction) override;
    void stopGame() override;
    void startGameLoop();
};
