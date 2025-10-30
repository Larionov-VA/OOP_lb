#pragma once
#include "ISpell.hpp"
#include "../core/EntityManager.hpp"
#include "../core/FieldCell.hpp"


class UpdateSpell : public SpellScroll {
private:
    
public:
    bool cast(GameContext& ctx, int userIndex, int power);
    UpdateSpell() {};
    ~UpdateSpell() {};
};