#include <iostream>
#include <unistd.h>
#include "Game.hpp"
#include "./visual/FTXUIVisualizer.hpp"
#include "./visual/IVisualizer.hpp"
#include "./visual/NCURSESVisualizer.hpp"
#include "InputController.hpp"
#include "AudioPlayer.hpp"

void choiceVisualizer(IVisualizer*& vis) {
    int visChoice;
    std::cout << "[0] - FTXUI\n[1] - NCURSES\n";
    std::cin >> visChoice;
    if (visChoice) {
        vis = new NCURSESVisualizer();
    }
    else {
        vis = new FTXUIVisualizer();
    }
}

int main() {
    Game game;
    IVisualizer* vis;
    InputController inp;
    choiceVisualizer(vis);
    vis->setGameController(&game);
    vis->setInputController(&inp);
    AudioPlayer player;
    if (player.loadAndPlay("../sounds/main_theme.wav", true)) {
        player.setVolume(0.5f);
    }
    vis->display();
    player.stop();
    return 0;
}


