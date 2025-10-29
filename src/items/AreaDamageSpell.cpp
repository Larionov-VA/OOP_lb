#include "AreaDamageSpell.hpp"
#include "../core/GameContext.hpp"
#include "../core/EntityManager.hpp"
#include "../core/FieldCell.hpp"
#include "../entities/Entity.hpp"

bool AreaDamageSpell::cast(GameContext& ctx, int userIndex) {
    if (!countOfItem) {
        return false;
    }
    std::vector<int> enemyIndexes = ctx.entityManager.getIndexesWithEntity(Entity::entityType::ENEMY);
    int barracksIndex = ctx.entityManager.getIndexesWithEntity(Entity::entityType::BARRACKS)[0];
    int towerIndex = ctx.entityManager.getIndexesWithEntity(Entity::entityType::TOWER)[0];
    enemyIndexes.push_back(barracksIndex);
    enemyIndexes.push_back(towerIndex);
    bool hit = false;
    for (int enemyIndex : enemyIndexes) {
        if (ctx.cells[userIndex].getDistance(ctx.cells[enemyIndex]) <= baseDistance * powerOfSpell) {
            Entity* user = ctx.entityManager[userIndex];
            int userInt = user->getInt();
            ctx.entityManager[enemyIndex]->causeDamage((userInt + baseDamage) * (powerOfSpell + userInt/10));
            hit = true;
        }
    }
    return hit;
}