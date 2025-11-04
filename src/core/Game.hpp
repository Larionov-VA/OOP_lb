#pragma once
#include <unistd.h>
#include "GameField.hpp"
#include "entities/Player.hpp"
#include "IGameController.hpp"
#include "Config.hpp"
#include "SaveManager.hpp"
#define FILENAME "Game.hpp"

class Game : public IGameController{
private:
    GameField* field;
    int gameID = 0;
public:
    ~Game();
    void startNewGame() override;
    void ContinueGame(int gameID) override;
    void stopGame() override;
    void saveGame() override;
    void loadGame() override;
    void nextLevel() override;
    void playerLevelUp(char attribute) override;
    bool performAnAction(char playerAction) override;
    std::shared_ptr<PlayerData> getPlayerData() override;
    std::vector<EnemyData> getEnemyData() override;
    std::vector<wchar_t> getFieldData() override;
private:
    void deleteField();
};
