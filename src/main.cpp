#include <iostream>
#include <unistd.h>
#include "Game.hpp"
#include "Visualizer.hpp"

int main() {
    Game game;
    Visualizer vis;
    vis.setController(&game);
    vis.display();
    return 0;
}