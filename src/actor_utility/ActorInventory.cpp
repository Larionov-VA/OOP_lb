#include "ActorInventory.hpp"




void Inventory::addItem(std::shared_ptr<Item> item) {
    
}


std::shared_ptr<Item> Inventory::getItem(int itemIndexInInventory) {
    inventory[itemIndexInInventory].first = false;
    return inventory[itemIndexInInventory].second;
}


Inventory::Inventory() {
}


Inventory::~Inventory() {
}