#pragma once
#include "../core/Config.hpp"


class Health {
private:
    int currentHealth;
    int maxHealth;
public:
    void regenerate(int amount);
    void reduseCurrentHealth(int damage);
    void restoreHealth();
    void setMaxHealth(int newMaxHealth);
    void updateHealth(int str);
    int getCurrentHealth() const;
    int getMaxHealth() const;
    Health();
    Health(int health);
    ~Health();
};
