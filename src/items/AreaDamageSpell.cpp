#include "AreaDamageSpell.hpp"
#include "../core/GameContext.hpp"
#include "../core/EntityManager.hpp"
#include "../core/FieldCell.hpp"
#include "../entities/Entity.hpp"

bool AreaDamageSpell::cast(GameContext& ctx, int userIndex, int power) {
    if (!countOfItem) {
        return false;
    }
    std::vector<int> allEnemy;
    std::vector<int> enemyIndexes = ctx.entityManager.getIndexesWithEntity(Entity::entityType::ENEMY);
    std::vector<int> barracksIndexes = ctx.entityManager.getIndexesWithEntity(Entity::entityType::BARRACKS);
    int barracksIndex = -1;
    if (!barracksIndexes.empty()) {
        barracksIndex = barracksIndexes[0];
    }
    std::vector<int> towerIndexes = ctx.entityManager.getIndexesWithEntity(Entity::entityType::TOWER);
    int towerIndex = -1;
    if (!towerIndexes.empty()) {
        towerIndex = towerIndexes[0];
    }
    allEnemy = enemyIndexes;
    if (barracksIndex != -1) {
        allEnemy.push_back(barracksIndex);
    }
    if (towerIndex != -1) {
        allEnemy.push_back(towerIndex);
    }
    for (int enemyIndex : allEnemy) {
        if (ctx.cells[userIndex]->getDistance(*ctx.cells[enemyIndex]) <= baseDistance * powerOfSpell + power * 3) {
            Entity* user = ctx.entityManager[userIndex];
            int userInt = user->getInt();
            ctx.entityManager[enemyIndex]->causeDamage((userInt + baseDamage) * (powerOfSpell + userInt/10));
        }
    }
    return true;
}