#include "Enemy.hpp"


Enemy::Enemy() : Enemy(1) {}

Enemy::Enemy(int enemyLevel) {
    enemyHealth.setMaxHealth(10 * enemyLevel);
    enemyHealth.restoreHealth();
    enemyAttack.setAttack(5 * enemyLevel);
}

Enemy::~Enemy() {

}


int Enemy::getDamage() {
    return enemyAttack.getAttack();
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