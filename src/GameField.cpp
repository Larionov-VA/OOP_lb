#include "GameField.hpp"


GameField::GameField(unsigned width = 10, unsigned height = 10) {
    if (width > MAX_FIELD_SIZE || height > MAX_FIELD_SIZE) {
        throw std::range_error("Max size of field is 25");
    }
    if (width < MIN_FIELD_SIZE || height < MIN_FIELD_SIZE) {
        throw std::range_error("Min size of field is 10");
    }
    weightField = width;
    heightField = height;
    cells.resize(width * height);
}

GameField::~GameField() {

}

void GameField::show() {
    for (unsigned i = 0; i < weightField * heightField; ++i) {
        std::cout << cells[i].getData();
        if ((i + 1) % weightField == 0) {
            std::cout << '\n';
        }
        else {
            std::cout << ' ';
        }
    }
}