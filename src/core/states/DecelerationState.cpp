#include "DecelerationState.hpp"

DecelerationState::DecelerationState() : DecelerationState(1, '&') {}

DecelerationState::DecelerationState(int duration, char symbol) {
    this->durationOfState = duration;
    this->stateSymbol = symbol;
}

void DecelerationState::incDuration() {
    ++this->durationOfState;
}

void DecelerationState::decDuration() {
    --this->durationOfState;
}

int DecelerationState::getDamage() {
    return this->damage;
}

void DecelerationState::setDamage(int damage) {
    this->damage = damage;
}

void DecelerationState::setDuration(int countOfRounds) const {
    this->durationOfState = countOfRounds;
}

int DecelerationState::getDuration() const {
    return this->durationOfState;
}

char DecelerationState::getSymbol() const {
    return this->stateSymbol;
}