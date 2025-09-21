#pragma once
#include "FieldCell.hpp"
#include <stdexcept>
#include <vector>
#include <cstdlib>
#include <iostream>
#include <ctime>
#define MAX_FIELD_SIZE 25
#define MIN_FIELD_SIZE 10

class GameField {
private:
    std::vector<FieldCell> cells{};
    unsigned widthField;
    unsigned heightField;
    void generateFieldCells();
public:
    GameField(unsigned weight, unsigned height);
    ~GameField();
    void show();
};

