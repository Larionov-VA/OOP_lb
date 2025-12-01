#pragma once



class CellState {
private:
    bool enemyCorpse;
    bool avaible;
    bool slow;
    bool trapped;
    int trapDamage;
public:
    CellState();
    CellState(bool enemyCorpse, bool avaible, bool slow, bool trapped, int trapDamage);
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
