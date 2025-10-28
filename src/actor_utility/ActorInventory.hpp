#pragma once
#include "../items/Item.hpp"
#include <vector>
#include <utility>
#include <memory>

class Inventory {
private:
    std::vector<std::unique_ptr<Item>> inventory;
public:
    Inventory();
    ~Inventory();
    void addItem();
};