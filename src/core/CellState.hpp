#pragma once
#include "./states/IState.hpp"
#include <memory>


class CellState {
private:
    std::unique_ptr<IState> constState;
    std::unique_ptr<IState> temporaryState;
    bool avaible;
    // bool enemyCorpse;
    // bool slow;
    // bool trapped;
    // int trapDamage;
public:
    CellState();
    CellState(std::unique_ptr<IState> constState, std::unique_ptr<IState> temporaryState);
    char getCellSymbol();
    bool haveSpecificState() const;
    void setConstState(std::unique_ptr<IState> constState);
    void setTemporaryState(std::unique_ptr<IState> temporaryState);
    // bool getEnemyCorpse() const;
    bool getAvaible() const;
    void setAvaible(bool newAvaible);
    int getStateDamage() const;
    // bool getSlow() const;
    // bool getTrapped() const;
    // int getTrapDamage() const;
    // void setEnemyCorpse(int newEnemyCorpse);
    // void setAvaible(int newAvaible);

    // void setSlow(int newSlow);

    // void setTrapped(int newTrapped);
    // void setTrapDamage(int newTrapDamage);
};
