#include <stdio.h>

#include "engine/game.h"
//#include "resource.rc"  //Icon Resource File

int main(int argc, char *argv[]) {
    game p_game;

    std::string l_level = "test.tmx";

    if( argc > 1) {
        l_level = argv[1];
    }

    return p_game.process_graphic( l_level);
}
