#include "ActorAttributes.hpp"


Attributes::Attributes(int intelligence, int dexterity, int strength) {
    this->intelligence = intelligence;
    this->dexterity = dexterity;
    this->strength = strength;
}


Attributes::Attributes(): Attributes(10,10,10) {};


Attributes::~Attributes() {
}


int Attributes::getIntelligence() const {
    return intelligence;
}


int Attributes::getDexterity() const {
    return dexterity;
}


int Attributes::getStrength() const {
    return strength;
}

void Attributes::setIntelligence(int newInt) {
    intelligence = newInt;
}

void Attributes::setDexterity(int newDex) {
    dexterity = newDex;
}

void Attributes::setStrength(int newStr) {
    strength = newStr;
}