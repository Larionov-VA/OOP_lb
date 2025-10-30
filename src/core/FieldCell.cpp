#include "FieldCell.hpp"


FieldCell::FieldCell(int index, unsigned X, unsigned Y, bool slow, bool avaible, bool dead, bool trapped, int trapDamage) {
    this->index = index;
    this->X = X;
    this->Y = Y;
    this->slow = slow;
    this->avaible = avaible;
    this->dead = dead;
    this->trapped = trapped;
    this->trapDamage = trapDamage;
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


void FieldCell::setTrap(int damage) {
    this->trapped = true;
    this->trapDamage = damage;
}


int FieldCell::checkAndSwitchTrap() {
    if (trapped) {
        this->trapped = false;
        return trapDamage;
    }
    return 0;
}


bool FieldCell::isCellAvaible() const {
    return avaible;
}


void FieldCell::setAvaible(bool avaible) {
    this->avaible = avaible;
}

bool FieldCell::checkCellDead() {
    if (dead) {
        dead = false;
        return true;
    }
    return false;
}

void FieldCell::setCellDead() {
    dead = true;
}

bool FieldCell::isTrapped() {
    return trapped;
}