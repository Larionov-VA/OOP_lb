#pragma once
#include "FieldCell.hpp"
#include "Entity.hpp"
#include "Player.hpp"
#include <stdexcept>
#include <vector>
#include <map>
#include <cstdlib>
#include <iostream>
#include <ctime>
#include <iomanip>
#include <memory>

#define MAX_FIELD_SIZE 25
#define MIN_FIELD_SIZE 10


class GameField {
private:
    std::vector<FieldCell> cells{};
    std::map<int, std::unique_ptr<Entity>> infoMap{};
    unsigned widthField;
    unsigned heightField;
    void generateFieldCells();
public:
    GameField(unsigned weight, unsigned height);

    void show();
};

