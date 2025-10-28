#include "ActorEquipment.hpp"


Equipment::Equipment() {
}


Equipment::~Equipment() {
}


int Equipment::getAttackBonus() {
    if (equippedWeapon == Equipment::equippedWeapon::RANGE) {
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
    rangeWeaponMulti = newMulti;
}


void Equipment::setRangeWeapon(int newMulti) {
    meleeWeaponMulti = newMulti;
}


bool Equipment::melle() const{
    return equippedWeapon == Equipment::equippedWeapon::MELEE;
}


bool Equipment::range() const {
    return equippedWeapon == Equipment::equippedWeapon::RANGE;
}


void Equipment::swapWeapon() {
    if (equippedWeapon == Equipment::equippedWeapon::RANGE) {
        equippedWeapon = Equipment::equippedWeapon::MELEE;
    }
    else {
        equippedWeapon = Equipment::equippedWeapon::RANGE;
    }
}