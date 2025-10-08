#include "ActorHealth.hpp"

Health::Health() : Health(100){}

Health::Health(int health) {
    currentHealth = health;
    maxHealth = health;
}


Health::~Health() {
}


void Health::reduseCurrentHealth(int damage) {
    currentHealth = currentHealth - damage;
}


void Health::regenerate(int regenerateAmount) {
    currentHealth = currentHealth + regenerateAmount;
}


void Health::restoreHealth() {
    currentHealth = maxHealth;
}


void Health::setMaxHealth(int newMaxHealth) {
    maxHealth = newMaxHealth;
}


int Health::getCurrentHealth() const {
    return currentHealth < 0 ? 0 : currentHealth;
}

int Health::getMaxHealth() const {
    return maxHealth;
}