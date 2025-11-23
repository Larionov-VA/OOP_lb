#include <ncurses.h>
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#include <atomic>
#include <thread>
#include <vector>
#include <string>

#include "../Config.hpp"
#include "IVisualizer.hpp"
#include "ASCIIAssets/ASCIIAssets.hpp"


class NCURSESVisualizer : public IVisualizer{
private:
    std::atomic<bool> running{true};
    int pin_states[6] = {0};
    IGameController* controller_ = nullptr;
public:
    void setController(IGameController* controller) override;
    void display() override;
    NCURSESVisualizer();
    ~NCURSESVisualizer();
};