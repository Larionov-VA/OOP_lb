#include "AttackEffect.hpp"

AttackEffect::AttackEffect() : AttackEffect('/', 0, 1) {}

AttackEffect::AttackEffect(char effectSympol, int effectDelay, int effectDuration) {
    this->stateSymbol = effectSympol;
    this->durationOfState = effectDuration + effectDelay;
    this->effectDelay = effectDelay;
}

void AttackEffect::incDuration() {
    --this->durationOfState;
    --this->effectDelay;
}

void AttackEffect::decDuration() {
    ++this->durationOfState;
    ++this->effectDelay;
}

int AttackEffect::getDamage() {
    return 0;
}

void AttackEffect::setDamage(int damage) {
    this->damage = damage;
}

void AttackEffect::setDuration(int countOfRounds) {
    this->durationOfState = countOfRounds;
}

int AttackEffect::getDuration() const {
    return this->durationOfState;
}

char AttackEffect::getSymbol() const {
    return this->stateSymbol;
}