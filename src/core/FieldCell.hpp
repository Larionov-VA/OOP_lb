#pragma once
#include <utility>
#include <cstdlib>
#include <cmath>
#include "CellState.hpp"


class FieldCell {
private:
    int index;
    int X;
    int Y;
    CellState state;
public:
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
    std::pair<int, int> getCoord() const;
    void setTrap(int damage);
    int checkAndSwitchTrap();
    bool isTrapped();
    bool isCellAvaible() const;
    void setAvaible(bool avaible);
    bool isCellSlow() const;
    void setSlow(bool avaible);
    bool checkCellDead();
    void setCellDead();
};
