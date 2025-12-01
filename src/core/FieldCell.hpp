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
        std::unique_ptr<IState> constState = nullptr,
        std::unique_ptr<IState> temporaryState = nullptr
    );
    int getIndex() const;
    char getCellSymbol();
    // void setCellState(std::unique_ptr<IState> constState, std::unique_ptr<IState> temporaryState);
    void setIndex(int index);
    float getDistance(const FieldCell& oth) const;
    std::pair<int, int> getCoord() const;
    // void setTrap(int damage);
    // int checkAndSwitchTrap();
    // bool isTrapped();
    // bool isCellAvaible() const;
    // void setAvaible(bool avaible);
    // bool haveState() const;
    CellState& returnCellState();
    // bool isCellSlow() const;
    // void setSlow(bool avaible);
    // bool checkCellDead();
    // void setCellDead();
};
