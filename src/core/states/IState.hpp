

class IState {
protected:
    int durationOfState;
    int damage;
    char stateSymbol;
public:
    virtual void incDuration() = 0;
    virtual void decDuration() = 0;

    virtual int getDamage() = 0;
    virtual void setDamage() = 0;

    virtual void setDuration(int countOfRounds) const = 0;
    virtual bool getDuration() const = 0;
    virtual char getSymbol() const = 0;
};