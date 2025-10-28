#pragma once

class Atributes {
private:
    int intelligence;
    int dexterity;
    int strength;
public:
    int getIntelligence() const;
    int getDexterity() const;
    int getStrength() const;
    void setIntelligence(int newInt);
    void setDexterity(int newDex);
    void setStrength(int newStr);
    Atributes(int intelligence, int dexterity, int strength);
    Atributes();
    ~Atributes();
};