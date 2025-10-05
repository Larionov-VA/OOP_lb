#pragma once
#include <stdexcept>
#include <vector>
#include <unordered_map>
#include <cstdlib>
#include <iostream>
#include <ctime>
#include <iomanip>
#include <memory>
#include <random>
#include "FieldCell.hpp"
#include "Entity.hpp"
#include "Player.hpp"

#define MAX_FIELD_SIZE 25
#define MIN_FIELD_SIZE 10


class GameField {
private:
    std::vector<FieldCell> cells{};
    std::unordered_map<int, std::unique_ptr<Entity>> infoMap{};
    int widthField;
    int heightField;
    unsigned gameLevel;
private:
    void generateFieldCells(std::unique_ptr<Entity> player);
    void moveEntity(int oldIndex, int newIndex);
    bool isMoveCorrect(int oldIndex, int newIndex) const;
    int getCellWithPlayer();
public:
    void playerTurn();
    void summonsTurn();
    void enemyTurn();
    void buildingsTurn();
    GameField(std::unique_ptr<Entity> player, int weight, int height, unsigned gameLevel);
    void show();
};