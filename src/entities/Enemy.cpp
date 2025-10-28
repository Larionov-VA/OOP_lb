#include "Enemy.hpp"


Enemy::Enemy() : Enemy(1) {}

Enemy::Enemy(int enemyLevel) {
    enemyHealth.setMaxHealth(GlobalGameConfig::enemyHealth * enemyLevel);
    enemyHealth.restoreHealth();
    enemyAttack.setAttack(GlobalGameConfig::enemyAttack * enemyLevel);
    iterative = true;
}

Enemy::~Enemy() {

}


int Enemy::getDamage() {
    return enemyAttack.getAttack();
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