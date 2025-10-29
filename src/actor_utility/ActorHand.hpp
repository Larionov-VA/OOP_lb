#pragma once
#include <memory>
#include <vector>
#include <random>

struct GameContext;
class Item;

class Hand {
private:
    std::shared_ptr<Item> itemInHand;
    std::vector<std::pair<bool, std::shared_ptr<Item>>> allItems;
    int maxSize;
    int currentSize;
public:
    Hand(int sizeOfHand);
    Hand();
    bool useItem(GameContext &ctx, int userIndex);
    void swapItem(int indexInHand);
    std::vector<std::pair<bool, int>> getHandData();
    int getCurrentHandSize();
    int getMaxHandSize();
    void addSpells(int indexInHand, int count);
private:
    void setRandomSpell();
    void setAvailableSpells();
};