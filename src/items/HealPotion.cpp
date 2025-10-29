#include "HealPotion.hpp"


void HealPotion::useItem(GameContext& ctx) {
    int playerIndex = ctx.entityManager.getIndexesWithEntity(Entity::entityType::PLAYER)[0];
    ctx.entityManager[playerIndex]->regenerateLife();
}