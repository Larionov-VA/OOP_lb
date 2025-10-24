#include "FieldCell.hpp"


FieldCell::FieldCell(int index, unsigned X, unsigned Y, bool slow, bool avaible) {
    this->index = index;
    this->X = X;
    this->Y = Y;
    this->slow = slow;
    this->avaible = avaible;
}


int FieldCell::getIndex() const {
    return index;
}


std::pair<int, int> FieldCell::getCoord() const {
    return {X,Y};
}


void FieldCell::setIndex(int index) {
    this->index = index;
}


// void FieldCell::swap(FieldCell &cell) {
//     int temp = cell.getIndex();
//     cell.setIndex(index);
//     this->setIndex(temp);
// }


bool FieldCell::isCellSlow() const {
    return slow;
}


void FieldCell::setSlow(bool slowState) {
    this->slow = slowState;
}


float FieldCell::getDistance(FieldCell oth) const {
    auto othCoord = oth.getCoord();
    
    float distance = pow((std::pow((std::abs(othCoord.first - X)), 2) + std::pow((std::abs(othCoord.second - Y)), 2)), 0.5);

    return distance;
}


bool FieldCell::isCellAvaible() const {
    return avaible;
}


void FieldCell::setAvaible(bool avaible) {
    this->avaible = avaible;
}