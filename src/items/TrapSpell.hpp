#pragma once
#include "ISpell.hpp"
#include "../core/EntityManager.hpp"
#include "../core/FieldCell.hpp"


class TrapSpell : public Item {
private:
    
public:
    bool cast(GameContext& ctx, int userIndex);
    TrapSpell() {};
    ~TrapSpell() {};
    void useItem(GameContext& ctx, int userIndex) override {
        (void)ctx;
        (void)userIndex;
    }
};
