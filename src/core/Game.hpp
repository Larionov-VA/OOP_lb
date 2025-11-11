#pragma once
#include <unistd.h>
#include "GameField.hpp"
#include "entities/Player.hpp"
#include "IGameController.hpp"
#include "Config.hpp"
#include "SaveManager.hpp"
#include "FileHandler.hpp"
#include "ISaveManager.hpp"
#include "SavesTree.hpp"
#define GAME_SAVES_DIR "/Game/"

class Game : public IGameController, public ISaveManager{
private:
    GameField* field;
    SavesTreeNode* head;
    int gameID = 0;
public:
    void addChild(ISaveManager* child) override { head->addChild(child); };
    void saveState(int saveID) override;
    void loadState(int loadID) override;
    char log() override { return 'G'; };
    SavesTreeNode* getSaveWrapper() { return head; }
    std::vector<ISaveManager*> getChilds() override { return head->getChilds(); };
public:
    Game();
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
    int getGameID();
    void setGameID(int newGameID);
    void deleteField();
};
