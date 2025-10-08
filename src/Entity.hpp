#pragma once
#include <utility>

class Entity {
public:
    enum class entityType { UNDEFINED, PLAYER, ENEMY, OBJECT };
    virtual ~Entity() = default;
    virtual entityType getType() const = 0;
    virtual void causeDamage(int damage) = 0;
    virtual int getDamage() = 0;
    virtual std::pair<int, int> getHealth() = 0;
    virtual bool alive() = 0;

    virtual void swapWeapon() {};
    virtual bool melle() { return true; };

    virtual int getInt() { return 0; };
    virtual int getDex() { return 0; };
    virtual int getStr() { return 0; };
};