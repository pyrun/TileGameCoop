#include "player.h"

#include <stdio.h>

player_handle::player_handle( config *config)
{
    p_config = config;
}

player_handle::~player_handle()
{
    // close connection
    for( int i = 0; i < (int)p_playerlist.size(); i++) {
        SDL_GameController *l_pad = p_playerlist[i]->controller;
        SDL_GameControllerClose(l_pad);
    }
}

void player_handle::handle( entitylist *entitylist) {
    int i = 0;
    for ( i = 0; i < SDL_NumJoysticks(); i++) {
        if (SDL_IsGameController(i) && !player_getDeviceNumber( i)) {

            SDL_GameController *l_controller = SDL_GameControllerOpen(i);

            if ( l_controller) {
                player_add( l_controller, i);
            } else {
                printf("Couldn't open SDL_GameController %d\n", i);
            }
        }
    }

    // react of push bottum
    for( int i = 0; i < (int)p_playerlist.size(); i++) {
        input_map *l_map;
        input_map *l_map_old;
        player* l_player = p_playerlist[i];
        SDL_GameController *l_pad = l_player->controller;

        // wechseln
        delete p_playerlist[i]->map_old;
        l_player->map_old = p_playerlist[i]->map;
        l_player->map = new input_map();

        // pointer
        l_map_old = l_player->map_old;
        l_map = l_player->map;

        l_map->x = SDL_GameControllerGetAxis( l_pad, (SDL_GameControllerAxis)p_config->getInputPadAxisX());
        l_map->y = SDL_GameControllerGetAxis( l_pad, (SDL_GameControllerAxis)p_config->getInputPadAxisY());

        // react
        l_map->jump = SDL_GameControllerGetButton( l_pad, (SDL_GameControllerButton)p_config->getInputPadButton_run() );
        l_map->run = SDL_GameControllerGetButton( l_pad, (SDL_GameControllerButton)p_config->getInputPadButton_jump() );
        l_map->attack = SDL_GameControllerGetButton( l_pad, (SDL_GameControllerButton)p_config->getInputPadButton_attack() );
        l_map->special = SDL_GameControllerGetButton( l_pad, (SDL_GameControllerButton)p_config->getInputPadButton_special() );

        // menu
        l_map->start = SDL_GameControllerGetButton( l_pad, (SDL_GameControllerButton)p_config->getInputPadButton_start());
        l_map->select = SDL_GameControllerGetButton( l_pad, (SDL_GameControllerButton)p_config->getInputPadButton_select() );

        // right/left
        l_map->left = SDL_GameControllerGetButton( l_pad, (SDL_GameControllerButton)p_config->getInputPadButton_left());
        l_map->right = SDL_GameControllerGetButton( l_pad, (SDL_GameControllerButton)p_config->getInputPadButton_right());

        if( l_map->start && !l_map_old->start)
            l_player->wantToJoin = true;
        if( l_player->wantToJoin && !l_player->active) {
            if( l_player->entity_id == -1) {
                std::vector<int> l_obj = entitylist->findPlayerObject();
                int l_id = -1;


                for( int y = 0; y < (int)l_obj.size(); y++) {
                    bool l_found = true;

                    for( int n = 0; n < (int)p_playerlist.size(); n++)
                        if( p_playerlist[n]->entity_id == l_obj[y])
                            l_found = false;

                    if( l_found) {
                        l_id = l_obj[y];
                        break;
                    }
                }

                if( l_id == -1)
                    break;

                l_player->entity_id = l_id;
                l_player->active = true;

            }
        }

        if( l_player->active) {
            entity *l_entity = entitylist->getEntity( l_player->entity_id);
            entitytype *l_type = l_entity->getType();

            if( l_type->lua_hasLoaded()) {
               if( l_map->jump)
                l_type->lua_jump();
            }

            //printf( "x%d y%d %d %d %d %d s%d b%d l%d r%d\n", l_map->x, l_map->y,l_map->jump, l_map->run, l_map->attack, l_map->special, l_map->start, l_map->select, l_map->left, l_map->right);
        }
    }
}

int player_handle::player_getPlayerActive() {
    int l_number = 0;
    for( int i = 0; i < (int)p_playerlist.size(); i++) {
        if( p_playerlist[i]->active)
            l_number++;
    }
    return l_number;
}


void player_handle::player_add( SDL_GameController *controller, int device_number) {
    printf( "add controller %d\n", device_number );

    player *l_player =  new player();

    l_player->controller = controller;
    l_player->active = false;
    l_player->wantToJoin = false;
    l_player->device_number = device_number;

    p_playerlist.push_back( l_player);
}

bool player_handle::player_getDeviceNumber( int device_number) {
    for( int i = 0; i < (int)p_playerlist.size(); i++) {
        if( p_playerlist[i]->device_number == device_number)
            return true;
    }
    return false;
}
