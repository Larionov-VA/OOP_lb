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
    void nextLevel() override;
    bool performAnAction(char playerAction) override;
    std::shared_ptr<PlayerData> getPlayerData() override;
    std::vector<EnemyData> getEnemyData() override;
    std::vector<wchar_t> getFieldData() override;
    void stopGame() override;
};
