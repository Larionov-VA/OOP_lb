#pragma once
#include <utility>
#include <memory>
#include <vector>


class Entity {
public:
    enum class entityType { UNDEFINED, PLAYER, ENEMY, BARRACKS };
    virtual ~Entity() = default;
    virtual entityType getType() const = 0;
    virtual void causeDamage(int damage) = 0;
    virtual int getDamage() = 0;
    virtual std::pair<int, int> getHealth() = 0;
    virtual bool alive() const = 0;

    virtual void swapWeapon() {};
    virtual bool melle() { return true; };
    virtual bool checkDebaffState() { return false; };
    virtual void setDebaffState() {};


    virtual bool timeToSpawn() { return false; };

    virtual int getInt() { return 0; };
    virtual int getDex() { return 0; };
    virtual int getStr() { return 0; };
    virtual int getLevel() { return 1; };
    virtual void addExperience(int exp) { exp++; };
    virtual std::vector<long long> getExperience() { return {0,0,0}; };

    virtual std::unique_ptr<Entity> clone() const = 0;
};