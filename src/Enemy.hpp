#pragma once
#include "Entity.hpp"
#include "ActorAtributes.hpp"
#include "ActorAttack.hpp"
#include "ActorEquipment.hpp"
#include "ActorHealth.hpp"
#include "Config.hpp"
#include <utility>
#include <memory>


class Enemy : public Entity {
private:
    Attack enemyAttack;
    Health enemyHealth;
    bool iterative;
public:
    entityType getType() const override {
        return entityType::ENEMY;
    }
    int getDamage() override;
    std::pair<int, int>  getHealth() override;
    void causeDamage(int damage) override;
    bool alive() const override;
    Enemy();
    Enemy(int enemyLevel);
    ~Enemy();
    std::unique_ptr<Entity> clone() const override {
        return std::make_unique<Enemy>(*this);
    }
};
