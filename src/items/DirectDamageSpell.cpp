#include "DirectDamageSpell.hpp"
#include "../core/GameContext.hpp"
#include "../core/EntityManager.hpp"
#include "../core/FieldCell.hpp"
#include "../entities/Entity.hpp"

bool DirectDamageSpell::cast(GameContext& ctx) {
    if (!countOfItem) {
        return false;
    }
    
    auto playerIndex = ctx.entityManager.getIndexesWithEntity(Entity::entityType::PLAYER)[0];
    std::vector<int> enemyIndexes = ctx.entityManager.getIndexesWithEntity(Entity::entityType::ENEMY);
    
    bool hit = false;
    for (int enemyIndex : enemyIndexes) {
        if (ctx.cells[playerIndex].getDistance(ctx.cells[enemyIndex]) <= baseDistance * powerOfSpell) {
            Entity* player = ctx.entityManager[playerIndex];
            if (player) {
                int playerInt = player->getInt();
                ctx.entityManager[enemyIndex]->causeDamage((playerInt + baseDamage) * powerOfSpell);
                hit = true;
            }
        }
    }
    return hit;
}