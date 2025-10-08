#include "ActorAtributes.hpp"


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