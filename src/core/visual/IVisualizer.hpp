#pragma once
#include "../IGameController.hpp"

class IVisualizer {
public:
    virtual void setController(IGameController* controller) = 0;
    virtual void display() = 0;
};
