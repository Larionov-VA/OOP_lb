#include <unordered_map>
#include <memory>
#include <vector>
#include "Entity.hpp"

class EntityManager {
private:
    std::unordered_map<int, std::unique_ptr<Entity>> infoMap{};
public:
    void createEntity(std::unique_ptr<Entity>, int index);
    void killEntity(int index);
    void changeEntityIndex(int oldIndex, int newIndex);
    template<typename T> T getIndexesWithEntity(Entity::entityType type);
    std::unique_ptr<Entity> getEntity(int index);
    EntityManager();
    ~EntityManager();
};
