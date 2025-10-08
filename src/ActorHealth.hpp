#pragma once

class Health {
private:
    int health;
public:
    void setHealth(int attack);
    int getHealth() const;
    Health();
    Health(int health);
    ~Health();
};
