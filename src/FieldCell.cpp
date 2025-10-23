#include "FieldCell.hpp"


FieldCell::FieldCell(unsigned ID, unsigned X, unsigned Y, bool slow, bool avaible) {
    this->objectID = ID;
    this->X = X;
    this->Y = Y;
    this->slow = slow;
    this->avaible = avaible;
}


unsigned FieldCell::getID() const {
    return objectID;
}


std::pair<int, int> FieldCell::getCoord() const {
    return {X,Y};
}


void FieldCell::setID(unsigned ID) {
    objectID = ID;
}


void FieldCell::swap(FieldCell &cell) {
    unsigned temp = cell.getID();
    cell.setID(objectID);
    this->setID(temp);
}


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