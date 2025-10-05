#include "Player.hpp"

Player::Player(int playerAttack, int playerHealth) {
    this->playerAttack = Attack{playerAttack};
    this->playerHealth = Health{playerHealth};
}


Player::Player() {
    
}

bool Player::alive() {
    return playerHealth.getHealth();
}