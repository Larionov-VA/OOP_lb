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
    int powerUp;
    int maxSize;
    int currentSize;
public:
    Hand(int sizeOfHand);
    Hand();
    void updateSize();
    bool useItem(GameContext &ctx, int userIndex, int powerUp);
    void swapItem(int indexInHand);
    std::vector<std::pair<bool, int>> getHandData();
    int getCurrentHandSize();
    int getMaxHandSize();
    int getPowerOfSpell();
    void setPowerOfSpell(int newPower);
    void incPowerOfSpell();
    void addSpells(int indexInHand, int count);
    std::pair<int, int> getHandItemDamageAndDistance();
private:
    void setRandomSpell();
    void setAvailableSpells();
};