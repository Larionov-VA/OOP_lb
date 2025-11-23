#include <iostream>
#include <unistd.h>
#include "Game.hpp"
#include "./visual/FTXUIVisualizer.hpp"

int main() {
    Game game;
    FTXUIVisualizer vis;
    vis.setController(&game);
    vis.display();
    return 0;
}