#pragma once
#include <vector>
#include <unordered_set>
#include <iostream>
#include <memory>
#include <random>
#include <algorithm>
#include <string>
#include <queue>

#include "EntityManager.hpp"
#include "FieldCell.hpp"

#include "entities/Entity.hpp"
#include "entities/Player.hpp"
#include "entities/Enemy.hpp"
#include "entities/EnemyBarracks.hpp"
#include "entities/EnemyTower.hpp"

#include "Config.hpp"
#include "Visualizer.hpp"
#include "ISaveManager.hpp"
#include "SaveManager.hpp"
#include "SavesTree.hpp"

#define MAX_FIELD_SIZE 25
#define MIN_FIELD_SIZE 10

#define GAMEFIELD_SAVES_DIR "/Game/GameField/"

class GameField : public ISaveManager{
private:
    SavesTreeNode* head;
    std::vector<FieldCell> cells{};
    EntityManager entityManager{};
    int widthField;
    int heightField;
    int gameLevel;
    int gameTurn;
public:
    void addChild(ISaveManager* child) override { head->addChild(child); };
    void saveState(int saveID) override;
    void loadState(int loadID) override { (void)loadID; };
    char log() override { return 'F'; };
    std::vector<ISaveManager*> getChilds() override { return head->getChilds(); };
    SavesTreeNode* getSaveWrapper() { return head; }
private:
    int getFieldWidth();
    int getFieldHeight();
    int getGameLevel();
    int getGameTurn();
    void setFieldWidth(int newFieldWidth);
    void setFieldHeight(int newFieldHeight);
    void setGameLevel(int newGameLevel);
    void setGameTurn(int newGameTurn);
private:
    void generateFieldCells(std::unique_ptr<Entity> player);
    void moveEntity(int oldIndex, int newIndex);
    bool isMoveCorrect(int oldIndex, int newIndex) const;
    void spawnEntity(std::unique_ptr<Entity> entity, int index);
    void generateEnemy();
    int firstEnemyIndexOnLine(int oldIndex, int newIndex) const;
    int getBestTurnForEnemyRecursive(int indexEnemy, int playerIndex, std::unordered_map<int, int>& visited);
    int getBestTurnForEnemyPrimitive(int indexEnemy, int playerIndex);
    std::vector<std::pair<int, float>> getDistanceToPlayer(std::vector<int> enemyIndexes, int playerIndex);
    bool enemyOnIndex(int index) const;
public:
    ~GameField() { delete head; };
    GameField(
        std::vector<FieldCell> cells,
        EntityManager entityManager,
        int widthField,
        int heightField,
        int gameLevel,
        int gameTurn
    );
    GameField(std::unique_ptr<Entity> player, int weight, int height, int gameLevel);
    GameField(const GameField& other)
        : cells(other.cells),
          entityManager(other.entityManager),
          widthField(other.widthField),
          heightField(other.heightField),
          gameLevel(other.gameLevel),
          gameTurn(other.gameTurn)
          {}
    GameField& operator=(const GameField& other) {
        if (this == &other) return *this;
        cells = other.cells;
        entityManager = other.entityManager;
        widthField = other.widthField;
        heightField = other.heightField;
        gameLevel = other.gameLevel;
        gameTurn = other.gameTurn;
        return *this;
    }
    GameField(GameField&& other) noexcept
        : cells(std::move(other.cells)),
          entityManager(std::move(other.entityManager)),
          widthField(other.widthField),
          heightField(other.heightField),
          gameLevel(other.gameLevel),
          gameTurn(other.gameTurn)
          {}
    GameField& operator=(GameField&& other) noexcept {
        if (this == &other) return *this;
        cells = std::move(other.cells);
        entityManager = std::move(other.entityManager);
        widthField = other.widthField;
        heightField = other.heightField;
        gameLevel = other.gameLevel;
        gameTurn = other.gameTurn;
        return *this;
    }
    std::unique_ptr<Entity> returnPlayer();
    bool playerAlive() const;
    std::shared_ptr<PlayerData> getPlayerData();
    std::vector<EnemyData> getEnemyData();
    bool playerTurn(char command);
    void enemyTurn();
    void buildingsTurn();
    int getCountOfEnemy();
    void playerLevelUp(char attribute);
    void update();
    std::vector<wchar_t> show();
};
