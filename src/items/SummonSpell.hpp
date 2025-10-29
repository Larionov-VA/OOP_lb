#pragma once
#include "ISpell.hpp"
#include "../core/EntityManager.hpp"
#include "../core/FieldCell.hpp"


class SummonSpell : public Item {
private:
    int summonLevel;
public:
    bool cast(GameContext& ctx, int userIndex);
    SummonSpell(int level) {summonLevel = level;};
    SummonSpell() : SummonSpell(1) {};
    ~SummonSpell() {};
    void useItem(GameContext& ctx, int userIndex) override {
        (void)ctx;
        (void)userIndex;
    }
};
