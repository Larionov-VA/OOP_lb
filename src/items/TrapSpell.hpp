#pragma once
#include "ISpell.hpp"
#include "../core/EntityManager.hpp"
#include "../core/FieldCell.hpp"


class TrapSpell : public SpellScroll {
private:
    int trapLevel;
    int trapDamage;
public:
    bool cast(GameContext& ctx, int userIndex, int power) override;
    TrapSpell() : TrapSpell(1, 50){};
    TrapSpell(int trapLevel, int trapDamage) : trapLevel(trapLevel), trapDamage(trapDamage) {};
};
