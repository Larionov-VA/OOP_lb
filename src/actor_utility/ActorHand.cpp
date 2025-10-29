#include "ActorHand.hpp"
#include "../core/GameContext.hpp"
#include "../items/Item.hpp"
#include "../items/DirectDamageSpell.hpp"
#include "../items/AreaDamageSpell.hpp"
#include "../items/UpdateSpell.hpp"
#include "../items/SummonSpell.hpp"
#include "../items/TrapSpell.hpp"


Hand::Hand() : Hand(10) {}


Hand::Hand(int sizeOfHand) {
    setAvailableSpells();
    itemInHand = nullptr;
    setRandomSpell();
    this->maxSize = sizeOfHand;
    this->currentSize = 1;
}


bool Hand::useItem(GameContext &ctx, int userIndex) {
    if (itemInHand->getCountOfItem()) {
        itemInHand->useItem(ctx, userIndex);
        --currentSize;
        return true;
    }
    return false;
}


void Hand::swapItem(int indexInHand) {
    if (allItems[indexInHand].second == itemInHand) {
        return;
    }
    if ((int)allItems.size() <= indexInHand) {
        return;
    }
    for (auto& item : allItems) {
        if (item.second == allItems[indexInHand].second) {
            item.first = false;
        }
        if (item.second == itemInHand) {
            item.first = true;
        }
    }
    itemInHand = allItems[indexInHand].second;
}


void Hand::setRandomSpell() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, 4);
    int randomNumber = distrib(gen);
    allItems[randomNumber].second->incCountOfItem();
    swapItem(randomNumber);
}


void Hand::setAvailableSpells() {
    std::pair<bool, std::shared_ptr<Item>> areaSpell{true, std::make_shared<AreaDamageSpell>()};
    std::pair<bool, std::shared_ptr<Item>> directSpell{true, std::make_shared<DirectDamageSpell>()};
    std::pair<bool, std::shared_ptr<Item>> updateSpell{true, std::make_shared<UpdateSpell>()};
    std::pair<bool, std::shared_ptr<Item>> summonSpell{true, std::make_shared<SummonSpell>()};
    std::pair<bool, std::shared_ptr<Item>> trapSpell{true, std::make_shared<TrapSpell>()};
    allItems.push_back(areaSpell);
    allItems.push_back(directSpell);
    allItems.push_back(updateSpell);
    allItems.push_back(summonSpell);
    allItems.push_back(trapSpell);
}


std::vector<std::pair<bool, int>> Hand::getHandData() {
    std::vector<std::pair<bool, int>> data;
    for (auto& item : allItems) {
        std::pair<bool, int> currItem = {item.first, item.second->getCountOfItem()};
        data.push_back(currItem);
    }
    return data;
}


int Hand::getCurrentHandSize() {
    return currentSize;
}


int Hand::getMaxHandSize() {
    return maxSize;
}


void Hand::addSpells(int indexInHand, int count) {
    for (int i = 0; i < count; ++i) {
        allItems[indexInHand].second->incCountOfItem();
    }
}