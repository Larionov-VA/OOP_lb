#include "ActorAttack.hpp"

Attack::Attack() : Attack(10) {}

Attack::Attack(int attack) {
    this->attack = attack;
}

Attack::~Attack() {
}


int Attack::getAttack() const {
    return attack < 0 ? 0 : attack;
}

void Attack::setAttack(int attack) {
    this->attack = attack;
}