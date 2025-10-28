#pragma once
#include "Entity.hpp"
#include "../actor_utility/ActorAttributes.hpp"
#include "../actor_utility/ActorAttack.hpp"
#include "../actor_utility/ActorEquipment.hpp"
#include "../actor_utility/ActorHealth.hpp"
#include "../core/Config.hpp"
#include <utility>
#include <memory>


class Enemy : public Entity {
private:
    Attack enemyAttack;
    Health enemyHealth;
    int enemyLevel;
    bool iterative;
public:
    entityType getType() const override {
        return entityType::ENEMY;
    }
    int getDamage() override;
    int getLevel() override;
    std::pair<int, int>  getHealth() override;
    void causeDamage(int damage) override;
    bool getIterative();
    void setIterative(bool it);
    bool alive() const override;
    Enemy();
    Enemy(int enemyLevel);
    ~Enemy();
    std::unique_ptr<Entity> clone() const override {
        return std::make_unique<Enemy>(*this);
    }
};
