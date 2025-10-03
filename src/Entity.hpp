#pragma once
class Entity {
public:
    enum class Type { UNDEFINED, PLAYER, ENEMY, OBJECT };
    virtual ~Entity() = default;
    virtual Type getType() const = 0;
};
