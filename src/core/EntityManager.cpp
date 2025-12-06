#include "EntityManager.hpp"
#include "entities/Enemy.hpp"
#include "entities/EnemyBarracks.hpp"
#include "entities/Player.hpp"
#include "entities/EnemyTower.hpp"

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
    std::vector<int> enemyIndexes = getIndexesWithEntity(Entity::entityType::ENEMY);
    for (int index : enemyIndexes) {
        EnemySaveData enemyData = dynamic_cast<Enemy*>((Entity*)getEntity(index))->getEnemySaveData();
        enemyData.enemyIndex = index;
        data.enemyData.push_back(enemyData);
    }
    int playerIndex = getIndexesWithEntity(Entity::entityType::PLAYER)[0];
    PlayerSaveData playerData = dynamic_cast<Player*>((Entity*)getEntity(playerIndex))->getPlayerSaveData();
    data.playerData = playerData;
    int towerIndex = getIndexesWithEntity(Entity::entityType::TOWER)[0];
    TowerSaveData towerData = dynamic_cast<EnemyTower*>((Entity*)getEntity(towerIndex))->getTowerSaveData();
    data.towerData = towerData;
    int barrackIndexes = getIndexesWithEntity(Entity::entityType::BARRACKS)[0];
    BarrackSaveData barrackData = dynamic_cast<EnemyBarracks*>((Entity*)getEntity(barrackIndexes))->getBarrackSaveData();
    data.barrackData = barrackData;
    return data;
}