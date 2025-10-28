#include <unordered_map>
#include <memory>
#include <vector>
#include "entities/Entity.hpp"

class EntityManager {
private:
    std::unordered_map<int, std::unique_ptr<Entity>> infoMap{};
public:
    Entity* operator[](int index);
    const Entity* operator[](int index) const;
    void createEntity(std::unique_ptr<Entity> entity, int index);
    void killEntity(int index);
    void changeEntityIndex(int oldIndex, int newIndex);
    std::vector<int> getIndexesWithEntity(Entity::entityType type) const;
    Entity* getEntity(int index);
    std::unique_ptr<Entity> returnEntity(int index);
    EntityManager();
    ~EntityManager();
    EntityManager(const EntityManager& other) {
        for (const auto& [index, entity] : other.infoMap) {
            if (entity)
                infoMap[index] = entity->clone();
        }
    }
    EntityManager& operator=(const EntityManager& other) {
        if (this == &other) return *this;
        infoMap.clear();
        for (const auto& [index, entity] : other.infoMap) {
            if (entity)
                infoMap[index] = entity->clone();
        }
        return *this;
    }
    EntityManager(EntityManager&& other) noexcept : infoMap(std::move(other.infoMap)) {}

    EntityManager& operator=(EntityManager&& other) noexcept {
        if (this == &other) return *this;
        infoMap = std::move(other.infoMap);
        return *this;
    }
};
