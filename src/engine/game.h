#ifndef GAME_H
#define GAME_H

#include "../graphic/graphic.h"
#include "config.h"
#include "framenrate.h"
#include "input.h"
#include "player.h"
#include "world.h"
#include "font.h"
#include "entity.h"
#include <dirent.h>

class game {
    public:
        game();
        virtual ~game();

        void drawHUD();
        void loadTypes();

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
        entitylist *p_entity;

        timer p_timer;
        timer p_deltaTime;

        bool p_game_running;
};

#endif // GAME_H
