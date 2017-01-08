#ifndef GAME_H
#define GAME_H

#include "../graphic/graphic.h"
#include "config.h"
#include "framenrate.h"
#include "input.h"
#include "player.h"
#include "world.h"
#include "font.h"

class game {
    public:
        game();
        virtual ~game();

        int process();
        int process_graphic();

        graphic *getGraphic() { return p_graphic; }
    protected:

    private:
        graphic *p_graphic;
        config p_config;
        framenrate *p_framerate;
        input *p_input;
        player_handle *p_player;
        world *p_world;
        font *p_font;

        bool p_game_running;
};

#endif // GAME_H
