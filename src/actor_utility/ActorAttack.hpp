#pragma once
#include "../core/Config.hpp"

class Attack {
private:
    int attack;
public:
    void setAttack(int attack);
    int getAttack() const;
    Attack();
    Attack(int attack);
    ~Attack();
};
