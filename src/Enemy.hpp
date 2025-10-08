#pragma once
#include "Entity.hpp"
#include "ActorAtributes.hpp"
#include "ActorAttack.hpp"
#include "ActorEquipment.hpp"
#include "ActorHealth.hpp"
#include <utility>

class Enemy : public Entity {
private:
    Attack enemyAttack;
    Health enemyHealth;
public:
    entityType getType() const override {
        return entityType::ENEMY;
    }


    int getDamage() override;
    std::pair<int, int>  getHealth() override;
    void causeDamage(int damage) override;
    bool alive() override;
    Enemy();
    ~Enemy();
};
