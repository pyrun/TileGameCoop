#include <stdio.h>

#include "engine/game.h"
//#include "resource.rc"  //Icon Resource File


int main(int argc, char *argv[]) {
    game p_game;

    std::string l_level;

    for( int i = 0; i < (int)argc; i++) {
        std::string l_argv = argv[i];
        // check
        if( l_argv == "-level" && i+1 < (int)argc) {
            l_level = argv[ i+1];
            i++;
            continue;
        }
        if( l_argv == "-server") {
            p_game.getNetwork()->startServer();
            continue;
        }
        if( l_argv == "-client") {
            if( i+1 < (int)argc)
            {
                p_game.getNetwork()->startClient( argv[ i+1]);
                i++;
            }
                else
            {
                p_game.getNetwork()->startClient();
            }
            continue;
        }
    }

    return p_game.process_graphic( l_level);
}
