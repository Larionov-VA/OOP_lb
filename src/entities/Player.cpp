#include "Player.hpp"

Player::Player(
    int playerAttack, 
    int playerHealth, 
    int playerLevel,
    long long prevLevelUpExperience,
    long long playerExperience, 
    long long levelUpExperience,
    int playerIntelligence,
    int playerDexterity,
    int playerStrength
) {
    this->playerAttack = Attack{playerAttack};
    this->playerHealth = Health{playerHealth};
    this->playerStats = Stats{prevLevelUpExperience, playerExperience, levelUpExperience, playerLevel};
    this->playerAttributes = Atributes{playerIntelligence, playerDexterity, playerStrength};
}


Player::Player() {
    
}


bool Player::checkDebaffState() {
    if (slowed) {
        slowed = false;
        return true;
    }
    return false;
}


void Player::setDebaffState() {
    slowed = true;
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

std::vector<long long> Player::getExperience() {
    return playerStats.getExperience();
}

int Player::getLevel() {
    return playerStats.getLevel();
}

void Player::addExperience(int exp) {
    playerStats.addExperience(exp);
}