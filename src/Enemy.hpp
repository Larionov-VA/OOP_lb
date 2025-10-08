#pragma once
#include "Entity.hpp"
#include "ActorAtributes.hpp"
#include "ActorAttack.hpp"
#include "ActorEquipment.hpp"
#include "ActorHealth.hpp"

class Enemy : public Entity {
private:
    Attack enemyAttack;
    Health enemyHealth;
public:
    entityType getType() const override {
        return entityType::ENEMY;
    }

    void getDamage(int damage) {

    }

    bool alive() {
        return enemyHealth.getHealth();
    }
    Enemy();
    ~Enemy();
};
