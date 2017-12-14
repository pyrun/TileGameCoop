#ifndef PLAYER_H
#define PLAYER_H

//#include <SDL2/SDL.h>
#include <vector>

#include "input.h"
#include "config.h"
#include "entity.h"

#include "../graphic/graphic.h"

class player_data {
    public:
        std::string index;
        std::string data;
};

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

        bool next_player_entity( entitylist *entitylist, player *l_player, bool rotateDir = false);
        void handle( entitylist *entity, world* world, input *input, graphic* graphic, config* config);
        void draw( entitylist *entitylist, font *font, graphic* graphic);
        void join( entitylist *entitylist);
        int getPlayerAmount() { return (int)p_playerlist.size(); }
        int getPlayerActive();
        int getAmountPlayerChamps();
        void setAllInavtive();
        void setPlayerChamp( int id, int entity);

        int getPlayerChamp( int id);
        void setInactiv( player *player);

        player *getPlayer( int id);
        player *findActivePlayer();
        player *getPlayerByEntity( int id);

        void createChamps( entitylist *entitylist, vec2 start);

        void addData( std::string index, std::string data);
        player_data *getData( std::string index);
        void setData( std::string index, std::string data);
        std::vector<player_data> getPlayerData() { return p_data; }
    protected:
    private:
        std::vector<player*> p_playerlist;
        std::vector<player_data> p_data;

        void player_add( SDL_GameController *controller, bool join);
        void player_remove( int id);

        int p_count;
};

class sort_player_struct {
    public:
        int id;
        int x;
        bool operator < (const sort_player_struct& obj) const { return ( x < obj.x); }
};

void lua_player_setLink( player_handle *player, entitylist *list);
void lua_player_install( lua_State *state);
extern player_handle *lua_player;

#endif // PLAYER_H
