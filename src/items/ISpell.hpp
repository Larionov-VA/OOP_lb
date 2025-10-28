#pragma once
#include "Item.hpp"
#include "../core/Config.hpp"

class SpellScroll : public Item {
protected:
    int powerOfSpell;
    virtual bool cast(GameContext& ctx) = 0;
public:
    SpellScroll(int power) { powerOfSpell = power; }
    SpellScroll() : SpellScroll(1) {}
    virtual ~SpellScroll() = default;
    void upgrade() {
        powerOfSpell++;
    }
    void useItem(GameContext& ctx) override {
        if (cast(ctx)) {
            incCountOfItem();
        }
    }
};