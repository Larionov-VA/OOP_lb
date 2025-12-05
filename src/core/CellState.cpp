#include "CellState.hpp"

CellState::CellState() : CellState(nullptr, nullptr) {};


CellState::CellState(std::shared_ptr<IState> constState, std::shared_ptr<IState> temporaryState) {
    this->constState = move(constState);
    this->temporaryState = move(temporaryState);
}


bool CellState::getAvaible() const {
    return this->avaible;
}


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


char CellState::getCellSymbol() {
    if (temporaryState) {
        temporaryState->decDuration();
        char res = temporaryState->getSymbol();
        if (!temporaryState->getDuration()) {
            temporaryState = nullptr;
        }
        return res;
    }
    else {
        return constState->getSymbol();
    }
}


bool CellState::haveSpecificState() const {
    return this->constState || this->temporaryState;
}


void CellState::setConstState(std::shared_ptr<IState> constState) {
    this->constState = move(constState);
}


void CellState::setTemporaryState(std::shared_ptr<IState> temporaryState) {
    this->temporaryState = move(temporaryState);
}