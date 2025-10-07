#include "EntityManager.hpp"

EntityManager::EntityManager() {

}

EntityManager::~EntityManager() {

}

std::unique_ptr<Entity> EntityManager::getEntity(int index){
    if (infoMap.empty() || !infoMap[index]) {
        return nullptr;
    }
    return std::move(infoMap[index]);
}


template<typename T>
T EntityManager::getIndexesWithEntity(Entity::entityType type){
    if (infoMap.empty()) {
        return -1;
    }
    std::vector<int> indexesOfCellsWithEntity{};
    for (const auto& pair : infoMap) {
        if (pair.second->getType() == type) {
            indexesOfCellsWithEntity.push_back(pair.first);
        }
    }
    if (type == Entity::entityType::PLAYER) {
        return indexesOfCellsWithEntity[0];
    }
    else {
        return indexesOfCellsWithEntity;
    }
}