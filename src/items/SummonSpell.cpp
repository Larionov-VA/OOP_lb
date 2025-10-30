#include "SummonSpell.hpp"
#include "../core/GameContext.hpp"
#include "../core/EntityManager.hpp"
#include "../core/FieldCell.hpp"
#include "../entities/Entity.hpp"

bool SummonSpell::cast(GameContext& ctx, int userIndex, int power) {
    if (!countOfItem) {
        return false;
    }
    int up = userIndex - GlobalGameConfig::fieldWidth;
    int down = userIndex + GlobalGameConfig::fieldWidth;
    int left = userIndex - 1;
    int right = userIndex + 1;
    for (int i = 0; i < 1 + power; ++i) {
        std::unique_ptr<Entity> summon = std::make_unique<SummonedUnit>(summonLevel);
        if (ctx.cells[up].isCellAvaible()) {
            ctx.entityManager.createEntity(std::move(summon), up);
            ctx.cells[up].setAvaible(false);
            return true;
        }
        else if (ctx.cells[down].isCellAvaible()) {
            ctx.entityManager.createEntity(std::move(summon), down);
            ctx.cells[down].setAvaible(false);
            return true;
        }
        else if (ctx.cells[left].isCellAvaible()) {
            ctx.entityManager.createEntity(std::move(summon), left);
            ctx.cells[left].setAvaible(false);
            return true;
        }
        else if (ctx.cells[right].isCellAvaible()) {
            ctx.entityManager.createEntity(std::move(summon), right);
            ctx.cells[right].setAvaible(false);
            return true;
        }
    }
    return false;
}