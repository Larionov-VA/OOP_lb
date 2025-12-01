#pragma once
#include "IState.hpp"

class DecelerationState : public IState{
public:
    DecelerationState();
    DecelerationState(int duration, char symbol);
    void incDuration();
    void decDuration();

    int getDamage();
    void setDamage(int damage);

    void setDuration(int countOfRounds) const;
    bool getDuration() const ;
    char getSymbol() const;
};
