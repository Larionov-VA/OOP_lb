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
    int X;
    int Y;
public:
    std::pair<int, int> getCoord() const;
    FieldCell(int index, unsigned X, unsigned Y, bool slow, bool avaible = false, bool dead = false);
    int getIndex() const;
    void setIndex(int index);
    // void swap(FieldCell& cell);
    float getDistance(FieldCell oth) const;

    bool isCellAvaible() const;
    void setAvaible(bool avaible);
    bool isCellSlow() const;
    void setSlow(bool avaible);
    bool checkCellDead();
    void setCellDead();
};
 