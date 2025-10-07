#include <unordered_map>
#include <memory>
#include <vector>
#include "Entity.hpp"

class EntityManager {
private:
    std::unordered_map<int, std::unique_ptr<Entity>> infoMap{};
public:
    Entity* operator[](int index);
    const Entity* operator[](int index) const;
    void createEntity(std::unique_ptr<Entity> entity, int index);
    void killEntity(int index);
    void changeEntityIndex(int oldIndex, int newIndex);
    std::vector<int> getIndexesWithEntity(Entity::entityType type);
    std::unique_ptr<Entity> getEntity(int index);
    EntityManager();
    ~EntityManager();
};
