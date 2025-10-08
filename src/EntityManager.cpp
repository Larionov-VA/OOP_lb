#include "EntityManager.hpp"


EntityManager::EntityManager() {

}


EntityManager::~EntityManager() {

}


Entity* EntityManager::operator[](int index) {
    auto it = infoMap.find(index);
    if (it != infoMap.end()) {
        return it->second.get();
    }
    return nullptr;
}


const Entity* EntityManager::operator[](int index) const {
    auto it = infoMap.find(index);
    if (it != infoMap.end()) {
        return it->second.get();
    }
    return nullptr;
}


void EntityManager::createEntity(std::unique_ptr<Entity> entity, int index) {
    if (!infoMap[index]) {
        infoMap[index] = std::move(entity);
    }
}

void EntityManager::killEntity(int index) {
    if (infoMap[index]) {
        infoMap.erase(index);
    }
}

void EntityManager::changeEntityIndex(int oldIndex, int newIndex) {
    infoMap[newIndex] = std::move(infoMap[oldIndex]);
    infoMap.erase(oldIndex);
}


std::unique_ptr<Entity> EntityManager::getEntity(int index){
    if (infoMap.empty() || !infoMap[index]) {
        return nullptr;
    }
    return std::move(infoMap[index]);
}


std::vector<int> EntityManager::getIndexesWithEntity(Entity::entityType type) const {
    std::vector<int> indexesOfCellsWithEntity{};
    for (const auto& pair : infoMap) {
        if (pair.second->getType() == type) {
            indexesOfCellsWithEntity.push_back(pair.first);
        }
    }
    return indexesOfCellsWithEntity;
}