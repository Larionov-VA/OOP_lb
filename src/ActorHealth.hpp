#pragma once

class Health {
private:
    int currentHealth;
    int maxHealth;
public:
    void regenerate(int amount);
    void reduseCurrentHealth(int damage);
    void restoreHealth();
    void setMaxHealth(int newMaxHealth);
    int getCurrentHealth() const;
    int getMaxHealth() const;
    Health();
    Health(int health);
    ~Health();
};
