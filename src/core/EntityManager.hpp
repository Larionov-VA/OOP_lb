#pragma once
#include <unordered_map>
#include <memory>
#include <vector>
#include "entities/Entity.hpp"
#include "../core/GameContext.hpp"

class EntityManager {
private:
    std::unordered_map<int, std::unique_ptr<Entity>> infoMap{};
public:
    EntityManager(){};
    Entity* operator[](int index);
    const Entity* operator[](int index) const;
    void createEntity(std::unique_ptr<Entity> entity, int index);
    void killEntity(int index);
    void changeEntityIndex(int oldIndex, int newIndex);
    std::vector<int> getIndexesWithEntity(Entity::entityType type) const;
    Entity* getEntity(int index);
    std::unique_ptr<Entity> returnEntity(int index);
    const std::unordered_map<int, std::unique_ptr<Entity>>& returnInfoMap();
};
