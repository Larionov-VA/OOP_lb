#include "UpgradeSpell.hpp"
#include "../core/GameContext.hpp"
#include "../core/EntityManager.hpp"
#include "../core/FieldCell.hpp"
#include "../entities/Entity.hpp"

bool UpdateSpell::cast(GameContext& ctx, int userIndex, int power) {
    if (!countOfItem) {
        return false;
    }
    int newPowerOfSpells = ctx.entityManager[userIndex]->getPowerOfSpell() + 1;
    ctx.entityManager[userIndex]->setPowerOfSpell(newPowerOfSpells + 1 * power);
    return true;
}

SpellSaveData UpdateSpell::getSpellSaveData() {
    SpellSaveData data;
    data.countOfItem = this->countOfItem;
    data.powerOfSpell = this->powerOfSpell;
    return data;
}