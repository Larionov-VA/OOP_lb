#include "ActorHand.hpp"

ActorHand::ActorHand(std::unique_ptr<Item> randomSpell) {
}

ActorHand::~ActorHand() {
}

bool ActorHand::useItem() {
    this->itemInHand->useItem();
}