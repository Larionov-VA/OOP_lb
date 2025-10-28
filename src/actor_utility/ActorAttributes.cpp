#include "ActorAttributes.hpp"


Atributes::Atributes(int intelligence, int dexterity, int strength) {
    this->intelligence = intelligence;
    this->dexterity = dexterity;
    this->strength = strength;
}


Atributes::Atributes(): Atributes(10,10,10) {};


Atributes::~Atributes() {
}


int Atributes::getIntelligence() const {
    return intelligence;
}


int Atributes::getDexterity() const {
    return dexterity;
}


int Atributes::getStrength() const {
    return strength;
}

void Atributes::setIntelligence(int newInt) {
    intelligence = newInt;
}

void Atributes::setDexterity(int newDex) {
    dexterity = newDex;
}

void Atributes::setStrength(int newStr) {
    strength = newStr;
}