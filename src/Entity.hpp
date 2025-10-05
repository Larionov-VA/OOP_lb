#pragma once
class Entity {
public:
    enum class entityType { UNDEFINED, PLAYER, ENEMY, OBJECT };
    virtual ~Entity() = default;
    virtual entityType getType() const = 0;
};
