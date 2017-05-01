#include "player.h"

#include <stdio.h>

player_handle::player_handle()
{
    //p_config = config;

    p_playercamerafocus = NULL;
}

player_handle::~player_handle() {
    // close connection
    for( int i = 0; i < (int)p_playerlist.size(); i++) {
        SDL_GameController *l_pad = p_playerlist[i]->controller;
        SDL_GameControllerClose(l_pad);
    }
}

void player_handle::handle( entitylist *entitylist, input *input, graphic* graphic, config* config) {
    // handle new controler
    std::vector<int> l_device = input->getDevice();
    if( l_device.size() > 0) {
        for( int i = 0; i < (int)l_device.size(); i++) {

            if( !SDL_IsGameController( l_device[i] ) )
                continue;

            SDL_GameController *l_controller = SDL_GameControllerOpen(l_device[i]);
            if( !l_controller)
                continue;

            // Doppelte Controler auschließen
            for( int n = 0; n < (int)p_playerlist.size(); n++) {
                if( SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(l_controller)) == p_playerlist[n]->device_number) {
                    SDL_GameControllerClose( l_controller);
                    l_controller = NULL;
                    break;
                }
            }

            if( !l_controller)
                continue;

            // add controler
            player_add( l_controller);
        }
    }
    // delete old one
    l_device = input->getDeviceDestroyed();
    for( int i = 0; i < (int)l_device.size(); i++)
        player_remove(  l_device[i] );
    // clear input device list
    input->cearDevice();

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

        l_map->x = SDL_GameControllerGetAxis( l_pad, (SDL_GameControllerAxis)config->getInputPadAxisX());
        l_map->y = SDL_GameControllerGetAxis( l_pad, (SDL_GameControllerAxis)config->getInputPadAxisY());

        // x&y axies
        if( l_map->x > 32767/2)
            l_map->dir.right = true;
        else
            l_map->dir.right = false;
        if( l_map->x < -32767/2)
            l_map->dir.left = true;
        else
            l_map->dir.left = false;

        if( l_map->y > 32767/2)
            l_map->dir.down = true;
        else
            l_map->dir.down = false;
        if( l_map->y < -32767/2)
            l_map->dir.up = true;
        else
            l_map->dir.up = false;


        // get Buttons
        bool l_up = SDL_GameControllerGetButton( l_pad, SDL_CONTROLLER_BUTTON_DPAD_UP );
        bool l_down = SDL_GameControllerGetButton( l_pad, SDL_CONTROLLER_BUTTON_DPAD_DOWN );
        bool l_right = SDL_GameControllerGetButton( l_pad, SDL_CONTROLLER_BUTTON_DPAD_RIGHT );
        bool l_left = SDL_GameControllerGetButton( l_pad, SDL_CONTROLLER_BUTTON_DPAD_LEFT );

        // overwrite if press
        if( l_right && !l_map->dir.right)
            l_map->dir.right = true;
        if( l_left && !l_map->dir.left)
            l_map->dir.left = true;
        if( l_up && !l_map->dir.up)
            l_map->dir.up = true;
        if( l_down && !l_map->dir.down)
            l_map->dir.down = true;

        // react
        l_map->jump = SDL_GameControllerGetButton( l_pad, (SDL_GameControllerButton)config->getInputPadButton_run() );
        l_map->run = SDL_GameControllerGetButton( l_pad, (SDL_GameControllerButton)config->getInputPadButton_jump() );
        l_map->attack = SDL_GameControllerGetButton( l_pad, (SDL_GameControllerButton)config->getInputPadButton_attack() );
        l_map->special = SDL_GameControllerGetButton( l_pad, (SDL_GameControllerButton)config->getInputPadButton_special() );

        // menu
        l_map->start = SDL_GameControllerGetButton( l_pad, (SDL_GameControllerButton)config->getInputPadButton_start());
        l_map->select = SDL_GameControllerGetButton( l_pad, (SDL_GameControllerButton)config->getInputPadButton_select() );

        // right/left
        l_map->left = SDL_GameControllerGetButton( l_pad, (SDL_GameControllerButton)config->getInputPadButton_left());
        l_map->right = SDL_GameControllerGetButton( l_pad, (SDL_GameControllerButton)config->getInputPadButton_right());

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
                l_player->wantToJoin = false;
                if( p_playercamerafocus == NULL)
                    p_playercamerafocus = l_player;
            }
        }

        if( l_player->active) {
            entity *l_entity = entitylist->getEntity( l_player->entity_id);

            if( l_entity == NULL || l_entity->isAlive() == false || l_entity->getAction() == "die") {
                l_player->entity_id = -1;
                l_player->active = 0;
                continue;
            }

            if( l_entity->lua_hasLoaded()) {
                if( l_map->jump && !l_map_old->jump)
                    l_entity->lua_jump( l_entity->getId());
                if( l_map->run && !l_map_old->run)
                    l_entity->lua_run( l_entity->getId(), true);
                if( !l_map->run && l_map_old->run)
                    l_entity->lua_run( l_entity->getId(), false);

                // find next object
                if( l_map->right && !l_map_old->right) {
                    std::vector<int> l_obj = entitylist->findPlayerObject();
                    int l_id = -1;
                    int found_first = -1;

                    for( int y = 0; y < (int)l_obj.size(); y++) {
                        bool l_found = true;

                        for( int n = 0; n < (int)p_playerlist.size(); n++)
                            if( p_playerlist[n]->entity_id == l_obj[y])
                                l_found = false;

                        if( l_found ) {
                            if( l_obj[y] > l_player->entity_id) {
                                l_id = l_obj[y];
                                break;
                            }
                            if( found_first == -1)
                                found_first = l_obj[y];
                        }
                    }

                    if( l_id == -1 && found_first != -1)
                        l_id = found_first;
                    if( l_id == -1)
                        break;

                    l_player->entity_id = l_id;
                }

                if( l_map->select && !l_map_old->select)
                    config->setQuit( true);
                if( l_map->attack && !l_map_old->attack)
                    l_entity->lua_attack( l_entity->getId());
                if( l_map->special && !l_map_old->special)
                    l_entity->lua_special( l_entity->getId());

                if( l_map->dir.up )
                    l_entity->lua_up( l_entity->getId());
                if( l_map->dir.down )
                    l_entity->lua_down( l_entity->getId());
                if( l_map->dir.left )
                    l_entity->lua_left( l_entity->getId());
                if( l_map->dir.right )
                    l_entity->lua_right( l_entity->getId());

                // focus
                if( l_map->left && !l_map_old->left) {
                    if( p_playercamerafocus == l_player)
                        p_playercamerafocus = NULL;
                    else
                        p_playercamerafocus = l_player;
                } // end focus


            } //else if(l_type->getScriptName().length() > 1) l_entity->loadScript( l_type->getScriptName());

            //#printf( "x%d y%d %d %d %d %d s%d b%d l%d r%d\n", l_map->x, l_map->y,l_map->jump, l_map->run, l_map->attack, l_map->special, l_map->start, l_map->select, l_map->left, l_map->right);
            //printf( "%d %d %d %d\n", l_map->dir.right, l_map->dir.left, l_map->dir.up, l_map->dir.down);
        }
    }
    if( p_playercamerafocus != NULL) {
        entity *l_entity = entitylist->getEntity( p_playercamerafocus->entity_id);
        if( l_entity != NULL) {
            vec2 l_pos = l_entity->getPosition().tovec2();
            vec2 l_cam = graphic->getCameraSize();

            vec2 l_newpos = l_pos - (l_cam/vec2( 2.f, 2.f ));

            graphic->flyTo( l_newpos);
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


void player_handle::player_add( SDL_GameController *controller) {

    player *l_player =  new player();

    l_player->device_number = SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(controller));
    l_player->controller = controller;
    l_player->active = false;
    l_player->wantToJoin = false;

    p_playerlist.push_back( l_player);
}

void player_handle::player_remove( int id) {
    player *l_player = NULL;
    int i;
    for( i = 0; i < (int)p_playerlist.size(); i++) {
        if( p_playerlist[i]->device_number == id) {
            l_player = p_playerlist[i];
            break;
        }
    }

    if( l_player == NULL)
        return;

    if( p_playercamerafocus == l_player)
        p_playercamerafocus = NULL;

    // close the gamepad connection
    SDL_GameControllerClose( l_player->controller );

    // delete now the player
    p_playerlist.erase( p_playerlist.begin() + i);
}

