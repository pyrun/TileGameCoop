#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2\SDL.h>
#include <vector>

#include "input.h"
#include "config.h"
#include "entity.h"
#include "../graphic/graphic.h"

class player {
    public:
        player() { map = new input_map; map_old = new input_map; entity_id = -1;}
        ~player() { SDL_GameControllerClose(controller); }
        SDL_GameController *controller;
        bool active;
        bool wantToJoin;

        SDL_JoystickID device_number;

        input_map *map;
        input_map *map_old;

        int entity_id;
};

class player_handle
{
    public:
        player_handle( config *config);
        virtual ~player_handle();

        void handle( entitylist *entity, input *input, graphic* graphic);
        int getPlayerAmount() { return (int)p_playerlist.size(); }
        int player_getPlayerActive();
    protected:
    private:
        config *p_config;
        std::vector<player*> p_playerlist;

        void player_add( SDL_GameController *controller);
        void player_remove( int id);
        player *p_playercamerafocus;
};

#endif // PLAYER_H
