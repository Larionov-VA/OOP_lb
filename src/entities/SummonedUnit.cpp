#include "SummonedUnit.hpp"


bool SummonedUnit::alive() const {
    return unitHealth.getCurrentHealth();
}


void SummonedUnit::causeDamage(int damage) {
    unitHealth.reduseCurrentHealth(damage);
}


int SummonedUnit::getDamage() {
    return unitAttack.getAttack() * unitLevel;
}


std::pair<int, int> SummonedUnit::getHealth() {
    return std::make_pair(unitHealth.getCurrentHealth(), unitHealth.getMaxHealth());
}