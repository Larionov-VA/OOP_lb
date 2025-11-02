#include "HealPotion.hpp"


void HealPotion::useItem(GameContext& ctx, int userIndex, int power) {
    // только игрок может использовать HealPotion
    (void)userIndex;
    (void)power;
    int playerIndex = ctx.entityManager.getIndexesWithEntity(Entity::entityType::PLAYER)[0];
    ctx.entityManager[playerIndex]->regenerateLife();
}