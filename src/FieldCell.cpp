#include "FieldCell.hpp"


FieldCell::FieldCell(unsigned ID, unsigned X, unsigned Y, bool avaible) {
    objectID = ID;
    this->avaible = avaible;
    this->X = X;
    this->Y = Y;
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

int FieldCell::getDistance(FieldCell oth) const {
    auto othCoord = oth.getCoord();
    
    int distance = pow((std::pow((std::abs(othCoord.first - X)), 2) + std::pow((std::abs(othCoord.second - Y)), 2)), 0.5);

    return distance;
}

bool FieldCell::isCellAvaible() const {
    return avaible;
}

void FieldCell::setAvaible(bool avaible) {
    this->avaible = avaible;
}