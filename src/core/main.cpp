#include <iostream>
#include <unistd.h>
#include "Game.hpp"
#include "./visual/IVisualizer.hpp"
#include "./visual/NCURSESVisualizer.hpp"
#include "InputController.hpp"
#include "AudioPlayer.hpp"
#include "KeyboardLayout.hpp"
#include "CommandFactory.hpp"

int main() {
    InputController c;
    KeyboardLayout l{"../settings/keyboard.cfg"};
    CommandFactory f{c, l};
    while (true)
    {
        std::unique_ptr<Command> command = f.getCommand();
        if (command) {
            std::cout << command->getCommandName() << '\n';
        }
    }

    // Game game;
    // NCURSESVisualizer vis;
    // InputController inp;
    // vis.setGameController(&game);
    // vis.setInputController(&inp);
    // AudioPlayer player;
    // if (player.loadAndPlay("../sounds/main_theme.wav", true)) {
    //     player.setVolume(0.5f);
    // }
    // vis.display();
    // player.stop();
    return 0;
}
