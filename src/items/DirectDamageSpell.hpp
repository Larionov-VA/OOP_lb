#pragma once
#include "ISpell.hpp"
#include "../core/EntityManager.hpp"
#include "../core/FieldCell.hpp"

class DirectDamageSpell : public SpellScroll {
private:
    int baseDamage;
    int baseDistance;
    
public:
    DirectDamageSpell() : DirectDamageSpell(10, 7, 1, 0) {}
    DirectDamageSpell(int damage, int distance, int power, int count) {
        this->baseDamage = damage;
        this->baseDistance = distance;
        this->powerOfSpell = power;
        this->countOfItem = count;
    }
    ~DirectDamageSpell() override = default;
    bool cast(GameContext& ctx, int userIndex) override;
};
