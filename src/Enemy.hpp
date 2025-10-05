#pragma once
#include "Entity.hpp"

class Enemy : public Entity {
private:
    /* data */
public:
    entityType getType() const override {
        return entityType::ENEMY;
    }
    Enemy(/* args */);
    ~Enemy();
};
