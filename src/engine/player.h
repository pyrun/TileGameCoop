#ifndef PLAYER_H
#define PLAYER_H

//#include <SDL2/SDL.h>
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
        player_handle();
        virtual ~player_handle();

        void next_player_entity( entitylist *entitylist, player *l_player);
        void handle( entitylist *entity, input *input, graphic* graphic, config* config);
        int getPlayerAmount() { return (int)p_playerlist.size(); }
        int player_getPlayerActive();
        void setAllInavtive() {
            for( int i = 0; i < (int)p_playerlist.size(); i++) {
                player *p_player = p_playerlist[i];
                p_player->entity_id = -1;
                if( p_player->active == true )
                    p_player->wantToJoin = true;
                p_player->active = false;
            }
        }
        void resetEntitys() { p_entityNames.clear(); }
        void addEntity( std::string names) { p_entityNames.push_back( names); }
        std::vector<std::string> getEntityList() { return p_entityNames; }
    protected:
    private:
        std::vector<player*> p_playerlist;

        void player_add( SDL_GameController *controller);
        void player_remove( int id);
        player *p_playercamerafocus;

        std::vector<std::string> p_entityNames;
};

#endif // PLAYER_H
