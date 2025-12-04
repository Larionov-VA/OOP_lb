#pragma once
#include <utility>
#include <cstdlib>
#include <cmath>
#include "CellState.hpp"
#include "FileHandler.hpp"

#define CELL_SAVES_DIR "/Game/GameField/FieldCells/Cells/"

class FieldCell {
private:
    CellState state;
    int index;
    int X;
    int Y;
public:
    FieldCell(
        int index,
        int X,
        int Y,
        std::shared_ptr<IState> constState = nullptr,
        std::shared_ptr<IState> temporaryState = nullptr
    );
    int getIndex() const;
    char getCellSymbol();
    void setIndex(int index);
    float getDistance(const FieldCell& oth) const;
    std::pair<int, int> getCoord() const;
    CellState& returnCellState();
};
