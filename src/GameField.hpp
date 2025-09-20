#pragma once
#include "FieldCell.hpp"
#include <stdexcept>
#include <vector>
#include <iostream>
#define MAX_FIELD_SIZE 25
#define MIN_FIELD_SIZE 10

class GameField {
private:
    std::vector<FieldCell> cells;
    unsigned weightField;
    unsigned heightField;
public:
    GameField(unsigned weight, unsigned height);
    ~GameField();
    void show();
};

