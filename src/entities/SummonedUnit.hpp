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
    SummonedUnit() : SummonedUnit(1, 10, 100){};
    SummonedUnit(int level) : SummonedUnit(level, 10, 100){};
    SummonedUnit(int level, int attack, int maxHealth) : unitAttack(attack), unitHealth(maxHealth), unitLevel(level) {};
    bool alive() const override;
    void causeDamage(int damage) override;
    int getDamage() override;
    std::pair<int, int> getHealth() override;
    std::unique_ptr<Entity> clone() const override {
        return std::make_unique<SummonedUnit>(*this);
    }
};