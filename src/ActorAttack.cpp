#include "ActorAttack.hpp"

Attack::Attack() : Attack(10) {}

Attack::Attack(int attack) {
    this->attack = attack;
}

Attack::~Attack() {
}