#include "EnemyBarracks.hpp"

EnemyBarracks::EnemyBarracks(int barracksLevel, int spawnPeriod) {
    barracksHealth.setMaxHealth(GlobalGameConfig::barracksHealth * barracksLevel * GlobalGameConfig::difficulty);
    barracksHealth.restoreHealth();
    this->barracksLevel = barracksLevel;
    this->spawnPeriod = spawnPeriod;
}


EnemyBarracks::~EnemyBarracks() {
}


bool EnemyBarracks::timeToSpawn() {
    ++counter;
    if (spawnPeriod == counter) {
        counter = 0;
        return true;
    }
    return false;
}


int EnemyBarracks::getDamage() {
    return 0;
}


std::pair<int, int> EnemyBarracks::getHealth() {
    return {barracksHealth.getCurrentHealth(), barracksHealth.getMaxHealth()};
}


void EnemyBarracks::causeDamage(int damage) {
    barracksHealth.reduseCurrentHealth(damage);
}


bool EnemyBarracks::alive() const {
    return barracksHealth.getCurrentHealth();
}

int EnemyBarracks::getLevel() {
    return barracksLevel;
}