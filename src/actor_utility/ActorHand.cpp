#include "ActorHand.hpp"
#include "../core/GameContext.hpp"
#include "../items/Item.hpp"
#include "../items/DirectDamageSpell.hpp"

Hand::Hand(std::shared_ptr<Item> randomSpell) {
    itemInHand = randomSpell;
}

Hand::Hand() {
    itemInHand = std::make_shared<DirectDamageSpell>();
}

bool Hand::useItem(GameContext &ctx) {
    if (itemInHand) {
        itemInHand->useItem(ctx);
        return true;
    }
    return false;
}