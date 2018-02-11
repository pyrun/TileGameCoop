#ifndef GAME_H
#define GAME_H

#include "../graphic/graphic.h"
#include "../graphic/font.h"
#include "config.h"
#include "framenrate.h"
#include "input.h"
#include "player.h"
#include "world.h"
#include "entity.h"
#include "level.h"
#include "particle.h"
#include "audio.h"
#include "network.h"

#include <dirent.h>

class game {
    public:
        game();
        virtual ~game();

        void drawHUD();
        void loadTypes();

        int process_graphic( std::string levelName);

        network *getNetwork() { return p_network; }
        graphic *getGraphic() { return p_graphic; }
    protected:

    private:
        graphic *p_graphic;
        font *p_font;
        config *p_config;
        framenrate *p_framerate;
        input *p_input;
        player_handle *p_player;
        particle_list *p_particles;
        audio *p_audio;
        network *p_network;

        level *p_level;

        bool p_game_running;
};

#endif // GAME_H
