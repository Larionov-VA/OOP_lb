#pragma once
#include "Entity.hpp"
#include "ActorAtributes.hpp"
#include "ActorAttack.hpp"
#include "ActorEquipment.hpp"
#include "ActorHealth.hpp"
#include <utility>
#include <memory>

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
    bool alive() const override;
    bool melle() override;
    int getDamage() override;
    std::pair<int, int> getHealth() override;
    void causeDamage(int damage) override;
    void swapWeapon() override;
    int getInt() override;
    int getDex() override;
    int getStr() override;
    std::unique_ptr<Entity> clone() const override {
        return std::make_unique<Player>(*this);
    }
};