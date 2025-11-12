#include "TrapSpell.hpp"
#include "../core/GameContext.hpp"
#include "../core/EntityManager.hpp"
#include "../core/FieldCell.hpp"
#include "../entities/Entity.hpp"

bool TrapSpell::cast(GameContext& ctx, int userIndex, int power) {
    if (!countOfItem) {
        return false;
    }
    int up = userIndex - GlobalGameConfig::fieldWidth;
    int down = userIndex + GlobalGameConfig::fieldWidth;
    int left = userIndex - 1;
    int right = userIndex + 1;
    int currentTrapDamage = this->trapDamage * trapLevel * powerOfSpell + power * this->trapDamage;
    if (ctx.cells[up]->isCellAvaible() && !ctx.cells[up]->isTrapped()) {
        ctx.cells[up]->setTrap(currentTrapDamage);
        return true;
    }
    else if (ctx.cells[down]->isCellAvaible() && !ctx.cells[down]->isTrapped()) {
        ctx.cells[down]->setTrap(currentTrapDamage);
        return true;
    }
    else if (ctx.cells[left]->isCellAvaible() && !ctx.cells[left]->isTrapped()) {
        ctx.cells[left]->setTrap(currentTrapDamage);
        return true;
    }
    else if (ctx.cells[right]->isCellAvaible() && !ctx.cells[right]->isTrapped()) {
        ctx.cells[right]->setTrap(currentTrapDamage);
        return true;
    }
    return false;
}