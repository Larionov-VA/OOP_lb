#include "EntityManager.hpp"


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
    auto it = infoMap.find(index);
    if (it == infoMap.end() || !it->second) {
        infoMap[index] = std::move(entity);
    }
}


void EntityManager::killEntity(int index) {
    if (infoMap[index]) {
        infoMap.erase(index);
    }
}


void EntityManager::changeEntityIndex(int oldIndex, int newIndex) {
    if (infoMap.find(newIndex) != infoMap.end()) {
        return;
    }
    auto it = infoMap.find(oldIndex);
    if (it == infoMap.end()) return;
    infoMap[newIndex] = std::move(it->second);
    infoMap.erase(it);
}


Entity* EntityManager::getEntity(int index) {
    auto it = infoMap.find(index);
    if (it == infoMap.end()) return nullptr;
    return it->second.get();
}


std::unique_ptr<Entity> EntityManager::returnEntity(int index) {
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


const std::unordered_map<int, std::unique_ptr<Entity>>& EntityManager::returnInfoMap() {
    return this->infoMap;
}


EntitySaveData EntityManager::getEntitySaveData() {
    EntitySaveData data;
    return data;
}