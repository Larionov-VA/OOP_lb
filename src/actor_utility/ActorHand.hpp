#pragma once
#include "../items/Item.hpp"
#include <memory>


class ActorHand {
private:
    std::unique_ptr<Item> itemInHand;
public:
    ActorHand(std::unique_ptr<Item> randomSpell);
    ~ActorHand();
    bool useItem();
};