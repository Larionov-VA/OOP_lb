#include "CellState.hpp"

CellState::CellState() : CellState(false, false, false, false, 0) {};


CellState::CellState(bool enemyCorpse, bool avaible, bool slow, bool trapped, int trapDamage) {
    this->enemyCorpse = enemyCorpse;
    this->avaible = avaible;
    this->slow = slow;
    this->trapped = trapped;
    this->trapDamage = trapDamage;
}


bool CellState::getEnemyCorpse() const {
    return this->enemyCorpse;
}


bool CellState::getAvaible() const {
    return this->avaible;
}


bool CellState::getSlow() const {
    return this->slow;
}


bool CellState::getTrapped() const {
    return this->trapped;
}


int CellState::getTrapDamage() const {
    return this->trapDamage;
}


void CellState::setEnemyCorpse(int newEnemyCorpse) {
    this->enemyCorpse = newEnemyCorpse;
}


void CellState::setAvaible(int newAvaible) {
    this->avaible = newAvaible;
}


void CellState::setSlow(int newSlow) {
    this->slow = newSlow;
}


void CellState::setTrapped(int newTrapped) {
    this->trapped = newTrapped;
}


void CellState::setTrapDamage(int newTrapDamage) {
    this->trapDamage = newTrapDamage;
}
