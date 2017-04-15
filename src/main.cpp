#include <stdio.h>

#include "engine/game.h"

int main(int argc, char *argv[]) {
    game p_game;

    std::string l_level;

    if( argc > 0) {
        l_level = argv[0];
    }

    return p_game.process_graphic( l_level);
}
