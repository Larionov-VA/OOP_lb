#include <iostream>
#include <unistd.h>
#include "Game.hpp"
#include "./visual/FTXUIVisualizer.hpp"
#include "./visual/IVisualizer.hpp"
#include "./visual/NCURSESVisualizer.hpp"

int main() {
    Game game;
    IVisualizer* vis;
    int visChoice;
    std::cout << "[0] - FTXUI\n[1] - NCURSES\n";
    std::cin >> visChoice;
    if (visChoice) {
        vis = new NCURSESVisualizer();
    }
    else {
        vis = new FTXUIVisualizer();
    }
    vis->setController(&game);
    vis->display();
    return 0;
}