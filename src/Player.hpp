#pragma once
#include "Entity.hpp"
#include "ActorAtributes.hpp"
#include "ActorAttack.hpp"
#include "ActorEquipment.hpp"
#include "ActorHealth.hpp"


class Player : public Entity {
private:
    Atributes playerAttributes;
    Attack playerAttack;
    Equipment playerEquipment;
    Health playerHealth;
    
public:
    entityType getType() const override {
        return entityType::PLAYER;
    }
    Player();
    Player(int playerAttack, int playerHealth);
    bool alive();
};