#include "TrapSpell.hpp"
#include "../core/GameContext.hpp"
#include "../core/EntityManager.hpp"
#include "../core/FieldCell.hpp"
#include "../entities/Entity.hpp"

bool TrapSpell::cast(GameContext& ctx, int userIndex) {
    if (!countOfItem) {
        return false;
    }
    (void)ctx;
    (void)userIndex;
    // std::vector<int> enemyIndexes = ctx.entityManager.getIndexesWithEntity(Entity::entityType::ENEMY);
    // for (int enemyIndex : enemyIndexes) {
    //     if (ctx.cells[userIndex].getDistance(ctx.cells[enemyIndex]) <= baseDistance * powerOfSpell) {
    //         Entity* user = ctx.entityManager[userIndex];
    //         int userInt = user->getInt();
    //         ctx.entityManager[enemyIndex]->causeDamage((userInt + baseDamage) * (powerOfSpell + userInt/10));
    //         return true;
    //     }
    // }
    return false;
}