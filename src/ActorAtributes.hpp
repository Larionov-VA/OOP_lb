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
    Atributes(int intelligence, int dexterity, int strength);
    Atributes();
    ~Atributes();
};