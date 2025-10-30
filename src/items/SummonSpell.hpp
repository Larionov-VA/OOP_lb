#pragma once
#include "ISpell.hpp"
#include "../core/EntityManager.hpp"
#include "../core/FieldCell.hpp"


class SummonSpell : public SpellScroll {
private:
    int summonLevel;
public:
    bool cast(GameContext& ctx, int userIndex, int power) override;
    SummonSpell(int level) {summonLevel = level;};
    SummonSpell() : SummonSpell(1) {};
    ~SummonSpell() {};
};
