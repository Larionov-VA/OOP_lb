#include "CellState.hpp"

CellState::CellState() : CellState(nullptr, nullptr) {};


CellState::CellState(std::unique_ptr<IState> constState, std::unique_ptr<IState> temporaryState) {
    this->constState = move(constState);
    this->temporaryState = move(temporaryState);
}


// bool CellState::getEnemyCorpse() const {
//     return this->enemyCorpse;
// }


bool CellState::getAvaible() const {
    return this->avaible;
}


// bool CellState::getSlow() const {
//     return this->slow;
// }


// bool CellState::getTrapped() const {
//     return this->trapped;
// }


// int CellState::getTrapDamage() const {
//     return this->trapDamage;
// }


// void CellState::setEnemyCorpse(int newEnemyCorpse) {
//     this->enemyCorpse = newEnemyCorpse;
// }


void CellState::setAvaible(bool newAvaible) {
    this->avaible = newAvaible;
}


int CellState::getStateDamage() const {
    int tDamage = 0;
    int cDamage = 0;
    if (temporaryState) {
        tDamage = temporaryState->getDamage();
    }
    if (constState) {
        cDamage = constState->getDamage();
    }
    return tDamage + cDamage;
}

// void CellState::setSlow(int newSlow) {
//     this->slow = newSlow;
// }


// void CellState::setTrapped(int newTrapped) {
//     this->trapped = newTrapped;
// }


// void CellState::setTrapDamage(int newTrapDamage) {
//     this->trapDamage = newTrapDamage;
// }

char CellState::getCellSymbol() const {
    if (temporaryState) {
        return temporaryState->getSymbol();
        temporaryState->decDuration();
    }
    else {
        return constState->getSymbol();
    }

}

bool CellState::haveSpecificState() const {
    return this->constState || this->temporaryState;
}

void CellState::setConstState(std::unique_ptr<IState> constState) {
    this->constState = move(constState);
}

void CellState::setTemporaryState(std::unique_ptr<IState> temporaryState) {
    this->temporaryState = move(temporaryState);
}