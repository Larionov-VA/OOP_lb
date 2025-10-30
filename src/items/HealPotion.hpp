#pragma once
#include "Item.hpp"
#include "../core/Config.hpp"

class HealPotion : public Item{
private:
    int levelOfPotion;
public:
    HealPotion(int level) { levelOfPotion = level; };
    HealPotion() : HealPotion(1) {};
    void useItem(GameContext& ctx, int userIndex, int power) override;
};