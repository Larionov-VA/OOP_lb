#include "Enemy.hpp"


Enemy::Enemy() : Enemy(1) {}

Enemy::Enemy(int enemyLevel) {
    this->enemyLevel = enemyLevel;
    enemyHealth.setMaxHealth(GlobalGameConfig::enemyHealth * enemyLevel * GlobalGameConfig::difficulty);
    enemyHealth.restoreHealth();
    enemyAttack.setAttack(GlobalGameConfig::enemyAttack * enemyLevel * GlobalGameConfig::difficulty);
    iterative = true;
}

Enemy::~Enemy() {

}


int Enemy::getDamage() {
    return enemyAttack.getAttack();
}

int Enemy::getLevel() {
    return this->enemyLevel;
}


bool Enemy::getIterative() {
    return iterative;
}

void Enemy::setIterative(bool it) {
    this->iterative = it;
}


std::pair<int, int> Enemy::getHealth() {
    return std::make_pair(enemyHealth.getCurrentHealth(), enemyHealth.getMaxHealth());
}


void Enemy::causeDamage(int damage) {
    enemyHealth.reduseCurrentHealth(damage);
}

bool Enemy::alive() const {
    return enemyHealth.getCurrentHealth();
}