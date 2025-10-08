#pragma once

class Equipment {
private:
    enum class equippedWeapon { MELEE, RANGE } equippedWeapon;
    int meleeWeaponMulti = 1.5;
    int rangeWeaponMulti = 1;
    int getMeleeAttackBonus();
    int getRangeAttackBonus();
public:
    Equipment();
    ~Equipment();
    int getAttackBonus();
    void setMeleeWeapon(int newMulti);
    void setRangeWeapon(int newMulti);
    bool melle() const;
    bool range() const;
    void swapWeapon();
};