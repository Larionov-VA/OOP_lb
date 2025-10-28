#pragma once
#include <memory>

struct GameContext;
class Item;

class Hand {
private:
    std::shared_ptr<Item> itemInHand;
public:
    Hand(std::shared_ptr<Item> randomSpell);
    Hand();
    bool useItem(GameContext &ctx);
};