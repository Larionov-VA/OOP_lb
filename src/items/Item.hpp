#pragma once

struct GameContext;

class Item {
protected:
    int countOfItem;
public:
    Item() : Item(1) {};
    Item(int count) { countOfItem = count; };
    int getCountOfItem() { return countOfItem; };
    void decCountOfItem() { --countOfItem; };
    void incCountOfItem() { ++countOfItem; };
    virtual void useItem(GameContext& ctx) = 0;
    virtual ~Item() = default;
};