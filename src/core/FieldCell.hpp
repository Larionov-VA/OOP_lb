#pragma once
#include <utility>
#include <cstdlib>
#include <cmath>

class FieldCell {
private:
    int index;
    bool avaible;
    bool slow;
    bool dead; // убрать логику в класс "Состояние клетки"
    bool trapped; //к сожалению, пока что реализовано так
    int trapDamage;
    int X;
    int Y;
public:
    std::pair<int, int> getCoord() const;
    FieldCell(
        int index,
        unsigned X,
        unsigned Y,
        bool slow,
        bool avaible = false,
        bool dead = false,
        bool trapped = false,
        int trapDamage = 0
    );
    int getIndex() const;
    void setIndex(int index);
    float getDistance(FieldCell oth) const;
    void setTrap(int damage);
    int checkAndSwitchTrap();
    bool isCellAvaible() const;
    void setAvaible(bool avaible);
    bool isCellSlow() const;
    void setSlow(bool avaible);
    bool checkCellDead();
    void setCellDead();
    bool isTrapped();
};
