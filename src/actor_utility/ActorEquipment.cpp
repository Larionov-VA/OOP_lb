#include "ActorEquipment.hpp"


Equipment::Equipment() {
}


Equipment::~Equipment() {
}


int Equipment::getAttackBonus() {
    if (currentWeapon == Equipment::equippedWeapon::RANGE) {
        return getRangeAttackBonus();
    }
    else {
        return getMeleeAttackBonus();
    }
}

int Equipment::getMeleeAttackBonus() {
    return meleeWeaponMulti;
}


int Equipment::getRangeAttackBonus() {
    return rangeWeaponMulti;
}


void Equipment::setMeleeWeapon(int newMulti) {
    meleeWeaponMulti = newMulti;
}


void Equipment::setRangeWeapon(int newMulti) {
    rangeWeaponMulti = newMulti;
}


bool Equipment::melle() const{
    return currentWeapon == Equipment::equippedWeapon::MELEE;
}


bool Equipment::range() const {
    return currentWeapon == Equipment::equippedWeapon::RANGE;
}


void Equipment::swapWeapon() {
    if (currentWeapon == Equipment::equippedWeapon::RANGE) {
        currentWeapon = Equipment::equippedWeapon::MELEE;
    }
    else {
        currentWeapon = Equipment::equippedWeapon::RANGE;
    }
}