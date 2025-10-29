#pragma once
#include "ISpell.hpp"
#include "../core/EntityManager.hpp"
#include "../core/FieldCell.hpp"


class UpdateSpell : public Item {
private:
    
public:
    bool cast(GameContext& ctx, int userIndex);
    UpdateSpell() {};
    ~UpdateSpell() {};
    void useItem(GameContext& ctx, int userIndex) override {
        (void)ctx;
        (void)userIndex;
    }
};