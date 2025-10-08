#include "Enemy.hpp"


Enemy::Enemy() {
    enemyHealth.setMaxHealth(10);
    enemyHealth.restoreHealth();
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