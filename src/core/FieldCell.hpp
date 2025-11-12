#pragma once
#include <utility>
#include <cstdlib>
#include <cmath>
#include "CellState.hpp"
#include "SaveManager.hpp"
#include "FileHandler.hpp"
#include "ISaveManager.hpp"
#include "SavesTree.hpp"

#define CELL_SAVES_DIR "/Game/GameField/FieldCells/Cells/"

class FieldCell : public ISaveManager {
private:
    // SavesTreeNode* head;
    CellState state;
    int index;
    int X;
    int Y;
public:
    void addChild(ISaveManager* child) override { (void)child; };
    void saveState(int saveID) override;
    void loadState(int loadID) override;
    char log() override { return 'C'; };
    SavesTreeNode* getSaveWrapper() { return nullptr; }
    std::vector<ISaveManager*> getChilds() override { return {}; };
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
