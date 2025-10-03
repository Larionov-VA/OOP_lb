#pragma once
#include <utility>
#include <cstdlib>
#include <cmath>

class FieldCell {
private:
    unsigned objectID;
    bool avaible;
    int X;
    int Y;
public:
    std::pair<int, int> getCoord() const;
    FieldCell(unsigned ID, unsigned X, unsigned Y, bool avaible = false);
    unsigned getID() const;
    void setID(unsigned ID);
    void swap(FieldCell& cell);
    int getDistance(FieldCell oth) const;
    bool isCellAvaible() const;
    void setAvaible(bool avaible);
};
 