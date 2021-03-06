#include "player.h"

#include <stdio.h>
#include <algorithm>    // std::rotate

/** LUA FUNCTION */

player_handle *lua_player = NULL;
entitylist *lua_entitylist_player = NULL;

static int lua_setPlayerData( lua_State *state) {
    std::string l_index;
    std::string l_data;
    player_data *l_playerdata = NULL;

    if( !lua_isstring( state, 1) || !lua_isstring( state, 2) ) {
        printf( "lua_setPlayerData call wrong argument\n");
        return 0;
    }

    l_index = lua_tostring( state, 1);
    l_data = lua_tostring( state, 2);

    printf( "lua_setPlayerData: %s %s\n", l_index.c_str(), l_data.c_str());

    lua_player->setData( l_index, l_data);
    return 0;
}

static int lua_getPlayerData( lua_State *state) {
    std::string l_index;
    player_data *l_playerdata = NULL;

    if( !lua_isstring( state, 1) ) {
        printf( "lua_getPlayerData call wrong argument\n");
        return 0;
    }

    // get index
    l_index = lua_tostring( state, 1);

    // look if exist
    l_playerdata = lua_player->getData( l_index);
    if( l_playerdata == NULL) {
        printf( "lua_getPlayerData index \"%s\" not found\n", l_index.c_str());
        lua_pushstring( state, "0");
        return 1;
    }

    lua_pushstring( state, l_playerdata->data.c_str());
    return 1;
}

static int lua_getActivePlayer( lua_State *state) {
    std::vector<int> l_playerId;
    std::vector<int> l_obj = lua_entitylist_player->findPlayerObject();

    for( int i = 0; i < (int)l_obj.size(); i++) {
        if( lua_player->getPlayerByEntity( l_obj[i]) != NULL) {
            lua_pushnumber( state, lua_player->getPlayerByEntity( l_obj[i])->id);
            return 1;
        }
    }
    return 0;
}

static int lua_setPlayerChamp( lua_State *state) {
    int l_entity;
    int l_id;

    if( !lua_isnumber( state, 1) || !lua_isnumber( state, 2) ) {
        printf( "lua_setPlayerChamp call wrong argument\n");
        return 0;
    }

    l_entity = lua_tonumber( state, 1);
    l_id = lua_tonumber( state, 2);

    lua_player->setPlayerChamp( l_id, l_entity);
    return 0;
}

static int lua_getPlayerChamp( lua_State *state) {
    int l_entity;
    int l_player_id;

    if( !lua_isnumber( state, 1) ) {
        printf( "lua_getPlayerChamp call wrong argument\n");
        return 0;
    }

    l_player_id = lua_tonumber( state, 1);

    l_entity = lua_player->getPlayerChamp( l_player_id);

    lua_pushnumber( state, l_entity);

    return 1;
}

static int lua_getAmountPlayerActive( lua_State *state) {
    int l_id;

    l_id = lua_player->getPlayerActive();

    lua_pushnumber( state, l_id);

    return 1;
}

static int lua_getPlayer( lua_State *state) {
    std::string l_name;
    int l_id;

    if( !lua_isnumber( state, 1) ) {
        printf( "lua_getPlayer call wrong argument\n");
        return 0;
    }

    l_id = lua_tonumber( state, 1);
    player *l_player = lua_player->getPlayerByEntity( l_id);

    if( l_player == NULL) {
        lua_pushnumber( state, -1);
        return 1;
    }

    int l_player_id = lua_player->getPlayerByEntity( l_id)->id;

    lua_pushnumber( state, l_player_id);

    return 1;
}

void lua_player_install( lua_State *state) {
    lua_pushcfunction( state, lua_setPlayerData);
    lua_setglobal( state, "setPlayerData");

    lua_pushcfunction( state, lua_getPlayerData);
    lua_setglobal( state, "getPlayerData");

    lua_pushcfunction( state, lua_getActivePlayer);
    lua_setglobal( state, "getActivePlayer");

    lua_pushcfunction( state, lua_setPlayerChamp);
    lua_setglobal( state, "setPlayerChamp");

    lua_pushcfunction( state, lua_getPlayerChamp);
    lua_setglobal( state, "getPlayerChamp");

    lua_pushcfunction( state, lua_getAmountPlayerActive);
    lua_setglobal( state, "getAmountPlayerActive");

    lua_pushcfunction( state, lua_getPlayer);
    lua_setglobal( state, "getPlayer");
}

void lua_player_setLink( player_handle* player, entitylist *list) {
    // set list
    lua_player = player;
    lua_entitylist_player = list;
}


player_handle::player_handle()
{
    //p_config = config;

    p_count = 1;

    // try to add lua lib
    lua_loader_add( "player_handle", &lua_player_install);
}

player_handle::~player_handle() {
    // close connection
    for( int i = 0; i < (int)p_playerlist.size(); i++) {
        SDL_GameController *l_pad = p_playerlist[i]->controller;
        SDL_GameControllerClose(l_pad);
    }
}

bool player_handle::next_player_entity( entitylist *entitylist, player *l_player, bool rotateDir) {
    std::vector<int> l_obj = entitylist->findPlayerObject();
    int l_id_serach = -1;
    int l_pos = 0; // obj position at array

    // sorting by x axis
    std::vector<sort_player_struct> l_obj_sort;
    for( int i = 0; i < (int)l_obj.size(); i++)
        l_obj_sort.push_back( sort_player_struct{ l_obj[i], (int)(entitylist->getEntity( l_obj[i])->getPosition().x) } );
    std::sort (l_obj_sort.begin(), l_obj_sort.end());

    // now replace
    l_obj.clear();
    for( int i = 0; i < (int)l_obj_sort.size(); i++)
        l_obj.push_back( l_obj_sort[i].id);

    // position
    for( int i = 0; i < (int)l_obj.size(); i++)
        if( l_player->entity_id == l_obj[i] )
            l_pos = i;

    // rotate the index
    std::rotate(l_obj.begin(),l_obj.begin()+l_pos,l_obj.end());

    // left or right swap
    if( rotateDir == true)
        std::reverse(l_obj.begin(),l_obj.end());

    // find next logic obj
    for( int y = 0; y < (int)l_obj.size(); y++)
    {
        bool l_found = true;
        // check if same as the start one
        for( int n = 0; n < (int)p_playerlist.size(); n++)
            if( p_playerlist[n]->entity_id == l_obj[y])
                l_found = false;
        // save this found
        if( l_found )
            l_id_serach = l_obj[y];
    }
    // not found
    if( l_id_serach == -1)
        return false;

    // found -> set
    l_player->entity_id = l_id_serach;
    return true;
}

void player_handle::handle( entitylist *entitylist, world* world, input *input, graphic* graphic, config* config) {
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
            //player_add( l_controller, config->getForceJoin()==1?true:false);
            player_add( l_controller, false);
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

        // wechseln der Input map
        delete p_playerlist[i]->map_old;
        l_player->map_old = p_playerlist[i]->map;
        l_player->map = new input_map();

        // pointer auf die Input map
        l_map_old = l_player->map_old;
        l_map = l_player->map;

        // get controller axis
        l_map->x = SDL_GameControllerGetAxis( l_pad, (SDL_GameControllerAxis)atoi( config->get( "x_axis", "input", "0").c_str()) );
        l_map->y = SDL_GameControllerGetAxis( l_pad, (SDL_GameControllerAxis)atoi( config->get( "y_axis", "input", "1").c_str()) );
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

        // get pushed buttons
        bool l_up = SDL_GameControllerGetButton( l_pad, SDL_CONTROLLER_BUTTON_DPAD_UP );
        bool l_down = SDL_GameControllerGetButton( l_pad, SDL_CONTROLLER_BUTTON_DPAD_DOWN );
        bool l_right = SDL_GameControllerGetButton( l_pad, SDL_CONTROLLER_BUTTON_DPAD_RIGHT );
        bool l_left = SDL_GameControllerGetButton( l_pad, SDL_CONTROLLER_BUTTON_DPAD_LEFT );

        // handle d-pad as axis
        if( l_right && !l_map->dir.right)
            l_map->dir.right = true;
        if( l_left && !l_map->dir.left)
            l_map->dir.left = true;
        if( l_up && !l_map->dir.up)
            l_map->dir.up = true;
        if( l_down && !l_map->dir.down)
            l_map->dir.down = true;

        // react
        l_map->jump = SDL_GameControllerGetButton( l_pad, (SDL_GameControllerButton)atoi( config->get( "run", "input", "0").c_str()) );
        l_map->run = SDL_GameControllerGetButton( l_pad, (SDL_GameControllerButton)atoi( config->get( "jump", "input", "2").c_str()) );
        l_map->attack = SDL_GameControllerGetButton( l_pad, (SDL_GameControllerButton)atoi( config->get( "attack", "input", "3").c_str()) );
        l_map->special = SDL_GameControllerGetButton( l_pad, (SDL_GameControllerButton)atoi( config->get( "special", "input", "1").c_str()) );

        // menu
        l_map->start = SDL_GameControllerGetButton( l_pad, (SDL_GameControllerButton)atoi( config->get( "start", "input", "6").c_str()) );
        l_map->select = SDL_GameControllerGetButton( l_pad, (SDL_GameControllerButton)atoi( config->get( "select", "input", "4").c_str()) );

        // right/left
        l_map->left = SDL_GameControllerGetButton( l_pad, (SDL_GameControllerButton)atoi( config->get( "left", "input", "9").c_str()) );
        l_map->right = SDL_GameControllerGetButton( l_pad, (SDL_GameControllerButton)atoi( config->get( "right", "input", "10").c_str()) );

        // handle active player
        if( l_player->active) {
            entity *l_entity = entitylist->getEntity( l_player->entity_id);


            if( l_entity == NULL || l_entity->isAlive() == false || l_entity->getAction() == "die") {
                l_player->entity_id = -1;
                //l_player->active = 0;
                continue;
            }

            if( l_entity->lua_hasLoaded()) {

                if( l_map->jump && !l_map_old->jump)
                    l_entity->lua_jump( l_entity->getId());
                if( l_map->run && !l_map_old->run)
                    l_entity->lua_run( l_entity->getId(), true);
                if( !l_map->run && l_map_old->run)
                    l_entity->lua_run( l_entity->getId(), false);
                if( l_map->select && !l_map_old->select) {
                    if( world->getFileName() == "worlds/overworld.tmx")
                        world->setLoadWorld( "menu.tmx");
                    else
                        printf( "%s\n",world->getFileName().c_str());
                }

                if( l_map->attack && !l_map_old->attack)
                    l_entity->lua_attack( l_entity->getId());
                if( l_map->special && !l_map_old->special)
                    l_entity->lua_special( l_entity->getId());

                if( l_map->dir.up ) {
                    l_entity->lua_up( l_entity->getId());
                    if( !l_map_old->dir.up)
                        l_entity->lua_up_once( l_entity->getId());
                }
                if( l_map->dir.down ) {
                    l_entity->lua_down( l_entity->getId());
                    if( !l_map_old->dir.down)
                        l_entity->lua_down_once( l_entity->getId());
                }
                if( l_map->dir.left ) {
                    l_entity->lua_left( l_entity->getId());
                    if( !l_map_old->dir.left)
                        l_entity->lua_left_once( l_entity->getId());
                }
                if( l_map->dir.right ) {
                    l_entity->lua_right( l_entity->getId());
                    if( !l_map_old->dir.right)
                        l_entity->lua_right_once( l_entity->getId());
                }

                if( l_map->start && !l_map_old->start)
                    if( l_player->champ.size() == 0)
                        setInactiv( l_player);

                if( l_map->left && !l_map_old->left)
                    next_player_entity( entitylist, l_player);
                if( l_map->right && !l_map_old->right)
                    next_player_entity( entitylist, l_player, true);
            }
        }

        // find player
        else if( l_map->start && !l_map_old->start && entitylist->getAmountPlayerObject() > getPlayerActive())
            l_player->wantToJoin = true;

        // if he want too join and is not active
        if( l_player->wantToJoin && !l_player->active) {
            // search a entity and bind
            if( l_player->entity_id == -1) {
                // find a next player entity
                next_player_entity( entitylist, l_player);

                if( l_player->entity_id == -1 )
                    break;
                l_player->active = true;
                l_player->wantToJoin = false;
            }
        }
    }

    // calculate the camera focus
    int l_amount = 0;
    fvec2 l_pos_camera;
    for( int i = 0; i < (int)p_playerlist.size(); i++) {
        player* l_player = p_playerlist[i];
        if( l_player->active && l_player->entity_id ) {
            entity *obj = entitylist->getEntity( l_player->entity_id);
            if( obj == NULL)
                continue;
            fvec2 l_pos = obj->getPosition();
            vec2 l_cam = graphic->getCameraSize();
            l_pos_camera = l_pos_camera + l_pos - (l_cam/vec2( 2.f, 2.f ));
            l_amount++;
        }
    }
    if( l_amount > 0) {
        l_pos_camera = fvec2( l_pos_camera.x/l_amount, l_pos_camera.y/l_amount );
        graphic->flyTo( l_pos_camera.tovec2());
    }
}

void player_handle::draw( entitylist *entitylist, font *font, graphic* graphic) {
    // react pushed button
    for( int i = 0; i < (int)p_playerlist.size(); i++) {
        player* l_player = p_playerlist[i];
        if( l_player->active) {
            entity *l_entity = entitylist->getEntity( l_player->entity_id);
            if( !l_entity)
                continue;
            char l_text[255];
            sprintf( l_text, "P%d", l_player->id);
            vec2 l_offset = { l_entity->getType()->getWidth()/2.f - 3, l_entity->getType()->getHeight()+8};
            font->drawMessage( graphic, l_text, l_entity->getPosition().tovec2() + l_offset, 0.5f, 255, false, true)  ;
        }
    }
}

void player_handle::join( entitylist *entitylist) {
    for( int i = 0; i < (int)p_playerlist.size(); i++) {
        player *l_player = p_playerlist[i];
        entitytype *l_type = entitylist->getType( l_player->champ );
        if( l_type)
            entitylist->create( l_type, vec2( 100, 100));
    }
}

int player_handle::getPlayerActive() {
    int l_number = 0;
    for( int i = 0; i < (int)p_playerlist.size(); i++) {
        if( p_playerlist[i]->active)
            l_number++;
    }
    return l_number;
}

int player_handle::getAmountPlayerChamps() {
    int amount = 0;
    for( int i = 0; i < (int)p_playerlist.size(); i++) {
        player *p_player = p_playerlist[i];
        if( p_player != NULL && p_player->champ.size() > 0)
            amount++;
    }
    return amount;
}

void player_handle::setAllInavtive() {
    for( int i = 0; i < (int)p_playerlist.size(); i++) {
        player *p_player = p_playerlist[i];
        p_player->entity_id = -1;
        if( p_player->active == true )
            p_player->wantToJoin = true;
        p_player->active = false;
    }
}

void player_handle::setPlayerChamp( int id, int entity) {
    player* l_player = getPlayer( id);
    if( l_player)
        l_player->entity_id = entity;
}

int player_handle::getPlayerChamp( int id) {
    player* l_player = getPlayer( id);
    if( l_player)
        return l_player->entity_id;
    return -1;
}

void player_handle::setInactiv( player *player) {
    player->entity_id = -1;
    player->active = false;
}

player *player_handle::getPlayer( int id) {
    for( int i = 0; i < (int)p_playerlist.size(); i++) {
        player *p_player = p_playerlist[i];
        if( p_player->id == id)
            return p_player;
    }
    return NULL;
}

player *player_handle::findActivePlayer() {
    for( int i = 0; i < (int)p_playerlist.size(); i++) {
        player *p_player = p_playerlist[i];
        if( p_player->active == true)
            return p_player;
    }
    return NULL;
}

player *player_handle::getPlayerByEntity( int id) {
    for( int i = 0; i < (int)p_playerlist.size(); i++) {
        player *p_player = p_playerlist[i];
        if( p_player->entity_id == id)
            return p_player;
    }
    return NULL;
}

void player_handle::createChamps( entitylist *entitylist, vec2 start) {
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


void player_handle::addData( std::string index, std::string data) {
    player_data l_data;

    // set data
    l_data.index = index;
    l_data.data = data;

    printf( "player_handle::addData no data found\n");

    p_data.push_back( l_data);
}

player_data *player_handle::getData( std::string index) {
    for( int i = 0; i < (int)p_data.size(); i++)
        if( p_data[i].index == index)
            return &p_data[i];
    return NULL;
}

void player_handle::setData( std::string index, std::string data) {
    for( int i = 0; i < (int)p_data.size(); i++) {
        if( p_data[i].index == index) {
            printf( "player_handle::setData found\n");
            p_data[i].data = data;
            return;
        }
    }
    // nothing found add Data
    addData( index, data);
}

void player_handle::player_add( SDL_GameController *controller, bool join) {
    player *l_player =  new player();

    // setze die Eigenschaften
    l_player->device_number = SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(controller));
    l_player->controller = controller;
    l_player->active = false;
    l_player->wantToJoin = join;
    l_player->id = p_count;

    p_count++;

    p_playerlist.push_back( l_player);
}

void player_handle::player_remove( int id) {
    player *l_player = NULL;
    int i;

    // find player
    for( i = 0; i < (int)p_playerlist.size(); i++) {
        if( p_playerlist[i]->device_number == id) {
            l_player = p_playerlist[i];
            break;
        }
    }
    if( l_player == NULL)
        return;

    // close the gamepad connection
    SDL_GameControllerClose( l_player->controller );

    // delete now the player in vector
    p_playerlist.erase( p_playerlist.begin() + i);
}

