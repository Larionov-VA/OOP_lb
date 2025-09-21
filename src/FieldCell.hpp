#pragma once
#include <utility>
#include <cstdlib>
#include <cmath>

class FieldCell {
private:
    unsigned objectID;
    int X;
    int Y;
public:
    std::pair<int, int> getCoord() const;
    FieldCell(unsigned ID, unsigned X, unsigned Y);
    unsigned getID() const;
    void setID(unsigned ID);
    void swap(FieldCell& cell);
    int getDistance(FieldCell oth) const;
};
 