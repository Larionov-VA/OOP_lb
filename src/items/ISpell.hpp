#pragma once
#include "Item.hpp"


class SpellScroll : public Item {
private:
    int powerOfSpell;
    virtual bool cast() = 0;
public:
    virtual void upgrade() = 0;
    bool useItem() override {
        return cast();
    };
};