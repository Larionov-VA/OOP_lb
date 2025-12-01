#include "FieldCell.hpp"


FieldCell::FieldCell(int index, int X, int Y,
    std::unique_ptr<IState> constState,
    std::unique_ptr<IState> temporaryState) {
    this->index = index;
    this->X = X;
    this->Y = Y;
    this->state.setConstState(move(constState));
    this->state.setTemporaryState(move(temporaryState));
    // this->state.setSlow(slow);
    // this->state.setAvaible(avaible);
    // this->state.setEnemyCorpse(dead);
    // this->state.setTrapped(trapped);
    // this->state.setTrapDamage(trapDamage);
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


// bool FieldCell::isCellSlow() const {
//     return state.getSlow();
// }


// void FieldCell::setSlow(bool slowState) {
//     this->state.setSlow(slowState);
// }


float FieldCell::getDistance(const FieldCell& oth) const {
    auto othCoord = oth.getCoord();
    int dX = std::abs(othCoord.first - X);
    int dY = std::abs(othCoord.second - Y);
    float distance = pow((std::pow(dX, 2) + std::pow(dY, 2)), 0.5);
    return distance;
}


// void FieldCell::setTrap(int damage) {
//     this->state.setTrapped(true);
//     this->state.setTrapDamage(damage);
// }


// int FieldCell::checkAndSwitchTrap() {
//     if (this->state.getTrapped()) {
//         this->state.setTrapped(false);
//         return this->state.getTrapDamage();
//     }
//     return 0;
// }


// bool FieldCell::isTrapped() {
//     return this->state.getTrapped();
// }


// bool FieldCell::isCellAvaible() const {
//     return this->state.getAvaible();
// }


// void FieldCell::setAvaible(bool avaible) {
//     this->state.setAvaible(avaible);
// }


// bool FieldCell::checkCellDead() {
//     if (this->state.getEnemyCorpse()) {
//         this->state.setEnemyCorpse(false);
//         return true;
//     }
//     return false;
// }


// void FieldCell::setCellDead() {
//     this->state.setEnemyCorpse(true);
// }

char FieldCell::getCellSymbol() {
    char res = '-';
    if (!state.getAvaible()) {
        res = '#';
    } else if (state.haveSpecificState()) {
        res = state.getCellSymbol();
    }
    return res;
}

// void FieldCell::setCellState(std::unique_ptr<IState> constState, std::unique_ptr<IState> temporaryState) {
//     if (constState) {
//         state.setConstState(move(constState));
//     }
//     if (temporaryState) {
//         this->state.setTemporaryState(move(temporaryState));
//     }
// }

// bool FieldCell::haveState() const {
//     return state.haveSpecificState();
// }

CellState& FieldCell::returnCellState() {
    return state;
}