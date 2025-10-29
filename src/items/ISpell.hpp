#pragma once
#include "Item.hpp"
#include "../core/Config.hpp"

class SpellScroll : public Item {
protected:
    int powerOfSpell;
    virtual bool cast(GameContext& context, int userIndex) {
        (void)context;
        (void)userIndex;
        return false;
    };
public:
    SpellScroll(int power) { powerOfSpell = power; }
    SpellScroll() : SpellScroll(1) {}
    virtual ~SpellScroll() = default;
    void upgrade() {
        powerOfSpell++;
    }
    void useItem(GameContext& context, int userIndex) override {
        if (cast(context, userIndex)) {
            this->decCountOfItem();
        }
    }
};