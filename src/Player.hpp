#pragma once
#include "Entity.hpp"

class Player : public Entity
{
private:
    /* data */
public:
    Type getType() const override {
        return Type::PLAYER;
    }
};