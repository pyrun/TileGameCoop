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
        player() { map = new input_map; map_old = new input_map; entity_id = -1; champ = ""; }
        ~player() { SDL_GameControllerClose(controller); }
        SDL_GameController *controller;
        bool active;
        bool wantToJoin;
        int id;

        SDL_JoystickID device_number;

        input_map *map;
        input_map *map_old;

        int entity_id;

        std::string champ;
};

class player_handle
{
    public:
        player_handle();
        virtual ~player_handle();

        void next_player_entity( entitylist *entitylist, player *l_player, bool rotateDir = false);
        void handle( entitylist *entity, world* world, input *input, graphic* graphic, config* config);
        void draw( entitylist *entitylist, font *font, graphic* graphic);
        void join( entitylist *entitylist);
        int getPlayerAmount() { return (int)p_playerlist.size(); }
        int getPlayerActive();
        int getAmountPlayerChamps();
        void setAllInavtive() {
            for( int i = 0; i < (int)p_playerlist.size(); i++) {
                player *p_player = p_playerlist[i];
                p_player->entity_id = -1;
                if( p_player->active == true )
                    p_player->wantToJoin = true;
                p_player->active = false;
            }
        }
        void setPlayerChamp( int id, std::string name) {
            player* l_player = getPlayer( id);
            if( l_player)
                l_player->champ = name;
        }
        std::string getPlayerChamp( int id) {
            player* l_player = getPlayer( id);
            if( l_player)
                return l_player->champ;
            return "";
        }
        void setInactiv( player *player) {
            player->entity_id = -1;
            player->active = false;
            if( player == p_playercamerafocus)
                p_playercamerafocus = findActivePlayer();
        }

        player *getPlayer( int id) {
            for( int i = 0; i < (int)p_playerlist.size(); i++) {
                player *p_player = p_playerlist[i];
                if( p_player->id == id)
                    return p_player;
            }
            return NULL;
        }
        player *findActivePlayer() {
            for( int i = 0; i < (int)p_playerlist.size(); i++) {
                player *p_player = p_playerlist[i];
                if( p_player->active == true)
                    return p_player;
            }
            return NULL;
        }
        player *getPlayerByEntity( int id) {
            for( int i = 0; i < (int)p_playerlist.size(); i++) {
                player *p_player = p_playerlist[i];
                if( p_player->entity_id == id)
                    return p_player;
            }
            return NULL;
        }
        void createChamps( entitylist *entitylist, vec2 start) {
            for( int i = 0; i < (int)p_playerlist.size(); i++) {
                player *p_player = p_playerlist[i];
                if( p_player->champ.size() > 0) {
                    entitytype *l_type = entitylist->getType( p_player->champ);
                    if( l_type) {
                        int l_id = entitylist->create( l_type, start);
                        p_player->entity_id = l_id;
                        p_player->active = true;
                    }
                    p_player->champ = "";
                }
            }
        }

    protected:
    private:
        std::vector<player*> p_playerlist;

        void player_add( SDL_GameController *controller);
        void player_remove( int id);
        player *p_playercamerafocus;

        int p_count;
};

void lua_player_setLink( player_handle *player);
void lua_player_install( lua_State *state);
extern player_handle *lua_player;

#endif // PLAYER_H
