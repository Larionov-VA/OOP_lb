#pragma once
#include "Entity.hpp"
#include "../actor_utility/ActorHealth.hpp"
#include "../core/Config.hpp"
#include <iostream>

class EnemyBarracks : public Entity {
private:
    Health barracksHealth{};
    int spawnPeriod;
    int counter = 0;
public:
    EnemyBarracks() : EnemyBarracks(1, 5) {};
    EnemyBarracks(int barracksLevel, int spawnPeriod = 5);
    ~EnemyBarracks();
    bool timeToSpawn() override;
    entityType getType() const override {
        return entityType::BARRACKS;
    }
    int getDamage() override;
    std::pair<int, int>  getHealth() override;
    void causeDamage(int damage) override;
    bool alive() const override;
    std::unique_ptr<Entity> clone() const override {
        return std::make_unique<EnemyBarracks>(*this);
    }
};