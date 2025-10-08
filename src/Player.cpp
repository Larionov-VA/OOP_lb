#include "Player.hpp"

Player::Player(int playerAttack, int playerHealth) {
    this->playerAttack = Attack{playerAttack};
    this->playerHealth = Health{playerHealth};
}


Player::Player() {
    
}


bool Player::alive() const {
    return playerHealth.getCurrentHealth();
}


int Player::getDamage() {
    int baseDamage = playerAttack.getAttack();
    int equippedWeaponIncrease = playerEquipment.getAttackBonus();
    int attributeBonus;
    if (playerEquipment.melle()) {
        attributeBonus = playerAttributes.getStrength();
    }
    else {
        attributeBonus = playerAttributes.getDexterity();
    }
    return (baseDamage + attributeBonus) * equippedWeaponIncrease;
}


std::pair<int, int> Player::getHealth() {
    return std::make_pair(playerHealth.getCurrentHealth(), playerHealth.getMaxHealth());
}


void Player::causeDamage(int damage) {
    playerHealth.reduseCurrentHealth(damage);
}


void Player::swapWeapon() {
    playerEquipment.swapWeapon();
}


int Player::getInt() {
    return playerAttributes.getIntelligence();
}


int Player::getDex() {
    return playerAttributes.getDexterity();
}


int Player::getStr() {
    return playerAttributes.getStrength();
}

bool Player::melle() {
    return playerEquipment.melle();
}