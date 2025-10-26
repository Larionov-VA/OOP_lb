#pragma once
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <ctime>
#include <iomanip>
#include <memory>
#include <random>
#include <algorithm>
#include <string>
#include <functional>
#include "EntityManager.hpp"
#include "FieldCell.hpp"
#include "Entity.hpp"
#include "Player.hpp"
#include "Visualizer.hpp"
#include "Enemy.hpp"
#include "EnemyBarracks.hpp"
#include "Config.hpp"

#define MAX_FIELD_SIZE 25
#define MIN_FIELD_SIZE 10


class GameField {
private:
    std::vector<FieldCell> cells{};
    EntityManager entityManager{};
    int widthField;
    int heightField;
    int gameLevel;
    int gameTurn;
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
public:
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
    void summonsTurn();
    void enemyTurn();
    void buildingsTurn();
    int getCountOfEnemy();
    void update();
    std::vector<wchar_t> show();
};