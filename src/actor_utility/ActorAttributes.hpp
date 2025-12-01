#pragma once

class Attributes {
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
    Attributes(int intelligence, int dexterity, int strength);
    Attributes();
    ~Attributes();
};