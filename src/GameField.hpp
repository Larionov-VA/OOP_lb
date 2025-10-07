#pragma once
// #include <stdexcept>
#include <vector>
#include <unordered_map>
// #include <cstdlib>
#include <iostream>
#include <ctime>
#include <iomanip>
#include <memory>
#include <random>
#include <algorithm>
#include "EntityManager.hpp"
#include "FieldCell.hpp"
#include "Entity.hpp"
#include "Player.hpp"
#include "Enemy.hpp"

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
    // int getCellWithEntity(Entity::entityType type);
    void spawnEntity(std::unique_ptr<Entity> entity, int index);
    void generateEnemy();
    int getBestTurnForEnemy(int indexEnemy, int playerIndex, std::unordered_map<int, int>& visited);
public:
    void playerTurn();
    void summonsTurn();
    void enemyTurn();
    void buildingsTurn();
    GameField(std::unique_ptr<Entity> player, int weight, int height, int gameLevel);
    void show();
};