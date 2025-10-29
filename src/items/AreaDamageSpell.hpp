#pragma once
#include "ISpell.hpp"
#include "../core/EntityManager.hpp"
#include "../core/FieldCell.hpp"

class AreaDamageSpell : public SpellScroll {
private:
    int baseDamage;
    int baseDistance;
public:
    AreaDamageSpell() : AreaDamageSpell(6, 4, 1, 1) {}
    AreaDamageSpell(int damage, int distance, int power, int count) {
        this->baseDamage = damage;
        this->baseDistance = distance;
        this->powerOfSpell = power;
        this->countOfItem = count;
    }
    ~AreaDamageSpell() override = default;
    bool cast(GameContext& ctx) override;
};
