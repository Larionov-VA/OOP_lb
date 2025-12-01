#pragma once
#include <IState.hpp>
#include <memory>


class CellState {
private:
    std::unique_ptr<IState> state;
    bool enemyCorpse;
    bool avaible;
    bool slow;
    bool trapped;
    int trapDamage;
public:
    CellState();
    CellState(bool enemyCorpse, bool avaible, bool slow, bool trapped, int trapDamage);
    char getCellSymbol() const;
    bool getEnemyCorpse() const;
    bool getAvaible() const;
    bool getSlow() const;
    bool getTrapped() const;
    int getTrapDamage() const;
    void setEnemyCorpse(int newEnemyCorpse);
    void setAvaible(int newAvaible);
    void setSlow(int newSlow);
    void setTrapped(int newTrapped);
    void setTrapDamage(int newTrapDamage);
};
