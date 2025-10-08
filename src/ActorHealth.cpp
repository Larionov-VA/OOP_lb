#include "ActorHealth.hpp"

Health::Health() : Health(100){}

Health::Health(int health) {
    this->health = health;
}


Health::~Health() {
}


int Health::getHealth() const {
    return health < 0 ? 0 : health;
}