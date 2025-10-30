#pragma once
#include "Entity.hpp"
#include "../actor_utility/ActorAttack.hpp"
#include "../actor_utility/ActorHealth.hpp"

class SummonedUnit : public Entity {
private:
    Attack unitAttack;
    Health unitHealth;
    int unitLevel;
public:
    entityType getType() const override {
        return entityType::SUMMONEDUNIT;
    }
    SummonedUnit() : SummonedUnit(10, 100, 1){};
    SummonedUnit(int attack, int maxHealth, int level) : unitAttack(attack), unitHealth(maxHealth), unitLevel(1) {};
    bool alive() const override;
    void causeDamage(int damage) override;
    int getDamage() override;
    std::pair<int, int> getHealth() override;
};