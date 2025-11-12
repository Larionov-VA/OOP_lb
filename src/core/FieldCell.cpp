#include "FieldCell.hpp"


FieldCell::FieldCell(int index, int X, int Y, bool slow,
    bool avaible, bool dead, bool trapped, int trapDamage) {
    head = new SavesTreeNode;
    this->index = index;
    this->X = X;
    this->Y = Y;
    this->state.setSlow(slow);
    this->state.setAvaible(avaible);
    this->state.setEnemyCorpse(dead);
    this->state.setTrapped(trapped);
    this->state.setTrapDamage(trapDamage);
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


bool FieldCell::isCellSlow() const {
    return state.getSlow();
}


void FieldCell::setSlow(bool slowState) {
    this->state.setSlow(slowState);
}


float FieldCell::getDistance(FieldCell oth) const {
    auto othCoord = oth.getCoord();
    int dX = std::abs(othCoord.first - X);
    int dY = std::abs(othCoord.second - Y);
    float distance = pow((std::pow(dX, 2) + std::pow(dY, 2)), 0.5);
    return distance;
}


void FieldCell::setTrap(int damage) {
    this->state.setTrapped(true);
    this->state.setTrapDamage(damage);
}


int FieldCell::checkAndSwitchTrap() {
    if (this->state.getTrapped()) {
        this->state.setTrapped(false);
        return this->state.getTrapDamage();
    }
    return 0;
}


bool FieldCell::isTrapped() {
    return this->state.getTrapped();
}


bool FieldCell::isCellAvaible() const {
    return this->state.getAvaible();
}


void FieldCell::setAvaible(bool avaible) {
    this->state.setAvaible(avaible);
}


bool FieldCell::checkCellDead() {
    if (this->state.getEnemyCorpse()) {
        this->state.setEnemyCorpse(false);
        return true;
    }
    return false;
}


void FieldCell::setCellDead() {
    this->state.setEnemyCorpse(true);
}


void FieldCell::saveState(int saveID) {
    std::string fullPathForSave = SAVES_PATH + std::to_string(saveID) + CELL_SAVES_DIR + std::to_string(index) + "_data.txt";
    FileHandler file{fullPathForSave, std::ios::out};
    file.write(std::to_string(state.getAvaible()) + '\n');
    file.write(std::to_string(state.getSlow()) + '\n');
    file.write(std::to_string(state.getEnemyCorpse()) + '\n');
    file.write(std::to_string(state.getTrapped()) + '\n');
    file.write(std::to_string(state.getTrapDamage()) + '\n');
    file.write(std::to_string(index) + '\n');
    file.write(std::to_string(X) + '\n');
    file.write(std::to_string(Y));

    FileHandler f{"LOG.txt", std::ios::out};
    f.write(std::to_string(index) + '\n');
}


void FieldCell::loadState(int loadID) {
    (void)loadID;
}