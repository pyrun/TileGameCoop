#include "entity.h"

#include <dirent.h>
#include <algorithm>
using namespace tinyxml2;

#ifndef XMLCheckResult
	#define XMLCheckResult(a_eResult) if (a_eResult != XML_SUCCESS) { printf("Error: %i\n", a_eResult); return a_eResult; }
#endif

/** LUA FUNCTION */

entitylist *lua_entitylist = NULL;
world *lua_worldlist = NULL;
graphic *lua_graphic = NULL;

#include <iostream>

static int lua_print(lua_State* state) {
    if( !lua_isnumber( state, 1) ) {
        printf( "lua_print call wrong argument\n");
        return 0;
    }

    // get obj
    int l_id = lua_tointeger( state, 1);
    entity *l_obj = lua_entitylist->getEntity( l_id);
    if( l_obj == NULL) {
        printf( "lua_print obj not found\n");
        return 0;
    }

    // process the input
    int nargs = lua_gettop( state)-1;
    std::string l_text;
    for (int i=1; i <= nargs; ++i) {
		l_text += lua_tostring( state, i+1);
    }

    // call entity_list to add a new msg
    lua_entitylist->message( l_id, l_text, 1.f, vec2(), false, 500); // ms

    // finish
    return 0;
}

static int lua_play_sound(lua_State* state) {
    std::string l_name;
    if( !lua_isnumber( state, 1) || !lua_isstring( state, 2) ) {
        printf( "lua_play_sound call wrong argument\n");
        return 0;
    }

    // get obj
    int l_id = lua_tointeger( state, 1);
    entity *l_obj = lua_entitylist->getEntity( l_id);
    if( l_obj == NULL) {
        printf( "lua_play_sound obj not found\n");
        return 0;
    }

    // get sound name
    l_name = lua_tostring( state, 2);

    entity_sound *l_sound = l_obj->getType()->getSound( l_name);
    if( l_sound == NULL) {
        printf( "lua_play_sound sound obj not found\n");
        return 0;
    }

    vec2 l_camera_size = lua_graphic->getCameraSize();

    if( l_camera_size.x != 0) {
        l_camera_size.x = l_camera_size.x/2;
        l_camera_size.y = l_camera_size.y/2;
    }

    // set camera position
    l_sound->sound->setCameraPosition( lua_graphic->getCamera().tovec2() + l_camera_size);

    // play the sound
    if( l_sound->sound)
        l_sound->play( l_obj->getPosition().tovec2());

    // finish
    return 0;
}

static int lua_message(lua_State* state) {
    int l_lifetime;
    bool l_asHUD;
    float l_size;
    vec2 l_offset;

    if( !lua_isnumber( state, 1) || !lua_isnumber( state, 2) || !lua_isnumber( state, 3) || !lua_isnumber( state, 4) || !lua_isboolean( state, 5) || !lua_isnumber( state, 6)) {
        printf( "lua_message call wrong argument\n");
        return 0;
    }

    // get obj
    int l_id = lua_tointeger( state, 1);
    entity *l_obj = lua_entitylist->getEntity( l_id);
    if( l_obj == NULL) {
        printf( "lua_message obj not found\n");
        return 0;
    }

    l_size = lua_tonumber( state, 2);
    l_offset = vec2( lua_tonumber( state, 3), lua_tonumber( state, 4));
    l_asHUD = lua_toboolean( state, 5);
    l_lifetime = lua_tonumber( state, 6);

    // process the input
    int nargs = lua_gettop( state)-6;
    std::string l_text;
    for (int i=1; i <= nargs; ++i) {
		l_text += lua_tostring( state, i+6);
    }

    // call entity_list to add a new msg
    lua_entitylist->message( l_id, l_text, l_size, l_offset, l_asHUD, l_lifetime);

    // finish
    return 0;
}

static int lua_setLoadLevel( lua_State *state) {
    std::string l_level;

    if( !lua_isstring( state, 1) || !lua_isboolean( state, 2) ) {
        printf( "lua_setLoadLevel call wrong argument\n");
        return 0;
    }

    l_level = lua_tostring( state, 1);

    lua_worldlist->setLoadWorld( l_level);
    return 0;
}

static int lua_end_level( lua_State *state) {
    bool l_addPlayer;

    if( !lua_isboolean( state, 1) ) {
        printf( "lua_end_level call wrong argument\n");
        return 0;
    }


    l_addPlayer = lua_toboolean( state, 1);

    lua_worldlist->setEndLevel( l_addPlayer);
    return 0;
}

static int lua_getTile( lua_State *state) {
    tile *l_tile;
    int l_pos_x;
    int l_pos_y;

    if( !lua_isnumber( state, 1) || !lua_isnumber( state, 2) ) {
        printf( "lua_getTile call wrong argument\n");
        return 0;
    }

    l_pos_x = lua_tointeger( state, 1);
    l_pos_y = lua_tointeger( state, 2);

    l_tile = lua_worldlist->getTile( lua_worldlist->getCollsionMap(), vec2( l_pos_x, l_pos_y) );
    if(l_tile == NULL)
        return 0;

    // type regist?
    if( l_tile->type) {
        lua_pushboolean( state, l_tile->type->left);
        lua_pushboolean( state, l_tile->type->right);
        lua_pushboolean( state, l_tile->type->down);
        lua_pushboolean( state, l_tile->type->top);
        lua_pushboolean( state, l_tile->type->liquid);
    } else { // no -> state
        lua_pushboolean( state, true);
        lua_pushboolean( state, true);
        lua_pushboolean( state, true);
        lua_pushboolean( state, true);
        lua_pushboolean( state, false);
    }

    // return id
    lua_pushinteger( state, l_tile->id);

    return 6;
}

static int lua_setSolid( lua_State *state) {
    entity *l_obj;
    int l_id;
    bool l_set;

    if( !lua_isnumber( state, 1) || !lua_isboolean( state, 2) ) {
        printf( "lua_setSolid call wrong argument\n");
        return 0;
    }

    l_id = lua_tointeger( state, 1);
    l_set = lua_toboolean( state, 2);

    l_obj = lua_entitylist->getEntity( l_id);
    if( l_obj == NULL) {
        printf( "lua_setSolid obj not found\n");
        return 0;
    }

    l_obj->setSolid( l_set);
    return 0;
}

static int lua_setAlpha( lua_State *state) {
    entity *l_obj;
    int l_id;
    int l_alpha;

    if( !lua_isnumber( state, 1) || !lua_isnumber( state, 2) ) {
        printf( "lua_setAlpha call wrong argument\n");
        return 0;
    }

    l_id = lua_tointeger( state, 1);
    l_alpha = lua_tointeger( state, 2);

    l_obj = lua_entitylist->getEntity( l_id);
    if( l_obj == NULL) {
        printf( "lua_setAlpha obj not found\n");
        return 0;
    }

    l_obj->setAlpha( l_alpha);
    return 0;
}

static int lua_sendSignal( lua_State *state) {
    entity *l_obj;
    int l_id;
    int l_fromId;
    std::string l_data;

    if( !lua_isnumber( state, 1) || !lua_isnumber( state, 2) || !lua_isstring( state, 3) ) {
        printf( "lua_sendSignal call wrong argument\n");
        return 0;
    }

    l_id = lua_tointeger( state, 1);
    l_fromId = lua_tointeger( state, 2);
    l_data = lua_tostring( state, 3);

    l_obj = lua_entitylist->getEntity( l_id);
    if( l_obj == NULL) {
        printf( "lua_sendSignal obj not found\n");
        return 0;
    }

    l_obj->lua_signal( l_id, l_fromId, l_data);
    return 0;
}

static int lua_howManyPlayerEntity( lua_State *state) {
    int l_entitys;

    l_entitys = lua_entitylist->findPlayerObject().size();

    lua_pushnumber( state, l_entitys);
    return 1;
}

static int lua_findObjects( lua_State *state) {
    int l_id;
    fvec2 l_pos;
    fvec2 l_rect;

    if( !lua_isnumber( state, 1) || !lua_isnumber( state, 2) || !lua_isnumber( state, 3) || !lua_isnumber( state, 4) || !lua_isnumber( state, 5)) {
        printf( "lua_findObjects call wrong argument\n");
        return 0;
    }

    l_id = lua_tointeger( state, 1);
    l_pos = fvec2( lua_tointeger( state, 2), lua_tointeger( state, 3));
    l_rect = fvec2( lua_tointeger( state, 4), lua_tointeger( state, 5));

    std::vector<int> l_ids = lua_entitylist->collision_boundingBoxRect( l_pos, l_rect, l_id);

    for( int i  = 0; i < (int)l_ids.size(); i++)
        lua_pushnumber( state, l_ids[i]);

    return (int)l_ids.size();
}

static int lua_isEnemy( lua_State *state) {
    entity *l_obj;
    entitytype *l_type;
    int l_id;

    if( !lua_isnumber( state, 1) ) {
        printf( "lua_isenemy call wrong argument\n");
        return 0;
    }

    l_id = lua_tointeger( state, 1);
    l_obj = lua_entitylist->getEntity( l_id);
    if( l_obj == NULL) {
        printf( "lua_isenemy obj not found\n");
        return 0;
    }

    l_type = l_obj->getType();
    if( l_type == NULL)
        return 0;

    // look if a player
    bool l_enemy = l_type->getIsEnemy();
    lua_pushboolean( state, l_enemy);
    return 1;
}

static int lua_isPlayer( lua_State *state) {
    entity *l_obj;
    entitytype *l_type;
    int l_id;

    if( !lua_isnumber( state, 1) ) {
        printf( "lua_isPlayer call wrong argument\n");
        return 0;
    }

    l_id = lua_tointeger( state, 1);
    l_obj = lua_entitylist->getEntity( l_id);
    if( l_obj == NULL) {
        printf( "lua_isPlayer obj not found\n");
        return 0;
    }

    l_type = l_obj->getType();
    if( l_type == NULL)
        return 0;

    // look if a player
    bool l_enemy = l_type->getIsPlayer();
    lua_pushboolean( state, l_enemy);
    return 1;
}

static int lua_getAmountPlayerChamps( lua_State *state) {
    int l_id;

    l_id = lua_entitylist->getAmountPlayerObject();

    lua_pushnumber( state, l_id);

    return 1;
}

static int lua_kill( lua_State *state) {
    entity *l_obj;
    int l_id;

    if( !lua_isnumber( state, 1) ) {
        printf( "lua_kill call wrong argument\n");
        return 0;
    }

    l_id = lua_tointeger( state, 1);
    l_obj = lua_entitylist->getEntity( l_id);
    if( l_obj == NULL) {
        printf( "lua_kill obj not found\n");
        return 0;
    }
    if( l_obj->getAction() != "die")
    // set to die
    l_obj->setAction( "die");
    return 0;
}

static int lua_delete( lua_State *state) {
    entity *l_obj;
    int l_id;

    if( !lua_isnumber( state, 1) ) {
        printf( "lua_delete call wrong argument\n");
        return 0;
    }

    l_id = lua_tointeger( state, 1);

    l_obj = lua_entitylist->getEntity( l_id);
    if( l_obj == NULL) {
        printf( "lua_delete obj not found\n");
        return 0;
    }
    l_obj->isbedelete = 1;

    //lua_entitylist->deleteObj( l_id);
    return 0;
}


static int lua_createObject( lua_State *state) {
    int l_id;
    std::string l_typeName;
    int l_x, l_y;

    if( !lua_isstring( state, 1) || !lua_isnumber( state, 2) || !lua_isnumber( state, 3) ) {
        printf( "lua_createObject call wrong argument\n");
        return 0;
    }
    l_typeName = lua_tostring( state, 1);
    l_x = lua_tointeger( state, 2);
    l_y = lua_tointeger( state, 3);

    entitytype *l_type;
    l_type = lua_entitylist->getType( l_typeName);
    if( l_type == NULL) {
        printf( "lua_createObject dont found type '%s'\n", l_typeName.c_str());
        return 0;
    }
    l_id = lua_entitylist->create( l_type, vec2( l_x, l_y) ); //lua_tointeger( state, 1);

    lua_pushnumber( state, l_id);

    return 1;
}

static int lua_isAlive( lua_State *state) {
    entity *l_obj;
    int l_id;
    bool l_alive;
    std::string l_name;

    if( !lua_isnumber( state, 1) ) {
        printf( "lua_isAlive call wrong argument\n");
        return 0;
    }

    l_id = lua_tointeger( state, 1);

    l_obj = lua_entitylist->getEntity( l_id);

    if( l_obj == NULL)
        l_alive = false;
    else
        l_alive = true;

    // tote objekte sind auch "tod"
    if( l_obj != NULL)
        l_alive = l_obj->isAlive();

    lua_pushboolean( state, l_alive);

    return 1;
}

static int lua_setAnimation( lua_State *state) {
    entity *l_obj;
    int l_id;
    std::string l_name;

    if( !lua_isnumber( state, 1) || !lua_isstring( state, 2) ) {
        printf( "lua_setAnimation call wrong argument\n" );
        return 0;
    }

    l_id = lua_tointeger( state, 1);
    l_name = lua_tostring( state, 2);

    l_obj = lua_entitylist->getEntity( l_id);
    if( l_obj == NULL) {
        printf( "lua_addVelocity obj not found\n");
        return 0;
    }

    l_obj->setAction( l_name);
    return 0;
}

static int lua_setAnimationDirection( lua_State *state) {
    entity *l_obj;
    int l_id;
    bool l_dir;

    if( !lua_isnumber( state, 1) || !lua_isboolean( state, 2) ) {
        printf( "lua_setAnimationDirection call wrong argument\n");
        return 0;
    }

    l_id = lua_tointeger( state, 1);
    l_dir = lua_toboolean( state, 2);

    l_obj = lua_entitylist->getEntity( l_id);
    if( l_obj == NULL) {
        printf( "lua_addVelocity obj not found\n");
        return 0;
    }

    l_obj->setDirection( l_dir);
    return 0;
}

static int lua_getAnimation( lua_State *state) {
    entity *l_obj;
    int l_id;
    std::string l_action;

    if( !lua_isnumber( state, 1)) {
        printf( "lua_getAnimation call wrong argument\n");
        return 0;
    }

    l_id = lua_tointeger( state, 1);

    l_obj = lua_entitylist->getEntity( l_id);
    if( l_obj == NULL) {
        printf( "lua_getAnimation obj not found\n");
        return 0;
    }

    l_action = l_obj->getAction();
    lua_pushstring( state, l_action.c_str() );
    return 1;
}

static int lua_getAnimationDirection( lua_State *state) {
    entity *l_obj;
    int l_id;
    bool l_dir;

    if( !lua_isnumber( state, 1)) {
        printf( "lua_getAnimationDirection call wrong argument\n");
        return 0;
    }

    l_id = lua_tointeger( state, 1);

    l_obj = lua_entitylist->getEntity( l_id);
    if( l_obj == NULL) {
        printf( "lua_getAnimationDirection obj not found\n");
        return 0;
    }

    l_dir = l_obj->getDirection();
    lua_pushboolean( state, (bool)l_dir );
    return 1;
}

static int lua_getFrame( lua_State *state) {
    entity *l_obj;
    int l_id;
    int l_frame;

    if( !lua_isnumber( state, 1)) {
        printf( "lua_getFrame call wrong argument\n");
        return 0;
    }

    l_id = lua_tointeger( state, 1);

    l_obj = lua_entitylist->getEntity( l_id);
    if( l_obj == NULL) {
        printf( "lua_getFrame obj not found\n");
        return 0;
    }

    l_frame = l_obj->getFrame();
    lua_pushnumber( state, l_frame );
    return 1;
}

static int lua_setPosition( lua_State *state) {
    entity *l_obj;
    int l_id;
    float l_x, l_y;
    if ( !lua_isnumber( state, 1) || !lua_isnumber( state, 2) || !lua_isnumber( state, 3)) {
        printf( "lua_setPosition call wrong argument\n");
        return 0;
    }

    l_id = lua_tointeger( state, 1);
    l_x = lua_tonumber( state, 2);
    l_y = lua_tonumber( state, 3);

    l_obj = lua_entitylist->getEntity( l_id);
    if( l_obj == NULL) {
        printf( "lua_setPosition obj not found\n");
        return 0;
    }
    l_obj->setPos( fvec2( l_x, l_y) );
    l_obj->setUpdate( true);

    return 0;
}

static int lua_addVelocity( lua_State *state) {
    entity *l_obj;
    int l_id;
    float l_x, l_y;
    if ( !lua_isnumber( state, 1) || !lua_isnumber( state, 2) || !lua_isnumber( state, 3)) {
        printf( "lua_addVelocity call wrong argument\n");
        return 0;
    }

    l_id = lua_tointeger( state, 1);
    l_x = lua_tonumber( state, 2);
    l_y = lua_tonumber( state, 3);

    l_obj = lua_entitylist->getEntity( l_id);
    if( l_obj == NULL) {
        printf( "lua_addVelocity obj not found\n");
        return 0;
    }
    l_obj->addVelocity( fvec2( l_x, l_y) );
    l_obj->setUpdate( true);

    return 0;
}

static int lua_setVelocityX( lua_State *state) {
    entity *l_obj;
    int l_id;
    float l_x;
    if ( !lua_isnumber( state, 1) || !lua_isnumber( state, 2)) {
        printf( "lua_setVelocityX call wrong argument\n");
        return 0;
    }

    l_id = lua_tointeger( state, 1);
    l_x = lua_tonumber( state, 2);

    l_obj = lua_entitylist->getEntity( l_id);
    if( l_obj == NULL) {
        printf( "lua_setVelocityX obj not found\n");
        return 0;
    }
    l_obj->setVelocity( fvec2( l_x, l_obj->getVelocity().y) );
    l_obj->setUpdate( true);

    return 0;
}

static int lua_getVelocity( lua_State *state) {
    entity *l_obj;
    int l_id;
    if ( !lua_isnumber( state, 1) ) {
        printf( "lua_getVelocity call wrong argument\n");
        return 0;
    }

    l_id = lua_tointeger( state, 1);


    l_obj = lua_entitylist->getEntity( l_id);
    if( l_obj == NULL) {
        printf( "lua_getVelocity obj not found\n");
        return 0;
    }
    lua_pushnumber( state, l_obj->getVelocity().x );
    lua_pushnumber( state, l_obj->getVelocity().y );

    l_obj->setUpdate( true);

    return 2;
}

static int lua_setVelocityY( lua_State *state) {
    entity *l_obj;
    int l_id;
    float l_y;
    if ( !lua_isnumber( state, 1) || !lua_isnumber( state, 2)) {
        printf( "lua_setVelocityY call wrong argument\n");
        return 0;
    }

    l_id = lua_tointeger( state, 1);
    l_y = lua_tonumber( state, 2);

    l_obj = lua_entitylist->getEntity( l_id);
    if( l_obj == NULL) {
        printf( "lua_setVelocityY obj not found\n");
        return 0;
    }
    l_obj->setVelocity( fvec2( l_obj->getVelocity().x, l_y) );
    l_obj->setUpdate( true);

    return 0;
}

static int lua_getColision( lua_State *state) {
    entity *l_obj;
    int l_id;
    std::string l_name;
    bool l_return = false;

    if ( !lua_isnumber( state, 1) || !lua_isstring( state, 2)) {
        printf( "lua_getColision call wrong argument\n");
        return 0;
    }

    l_id = lua_tointeger( state, 1);
    l_name = lua_tostring( state, 2);

    l_obj = lua_entitylist->getEntity( l_id);
    if( l_obj == NULL) {
        printf( "lua_getColision obj not found\n");
        return 0;
    }

    if( l_name == "down")
        l_return = l_obj->getColisionDown();
    else if( l_name == "up")
        l_return = l_obj->getColisionUp();
    else if( l_name == "right")
        l_return = l_obj->getColisionRight();
    else if( l_name == "left")
        l_return = l_obj->getColisionLeft();

    lua_pushboolean( state, l_return);

    return 1;
}

static int lua_getVertexHit( lua_State *state) {
    entity *l_obj;
    int l_id, l_vertexid;
    if ( !lua_isnumber( state, 1)  || !lua_isnumber( state, 2) ) {
        printf( "lua_getVertexHit call wrong argument\n");
        return 0;
    }

    l_id = lua_tointeger( state, 1);
    l_vertexid = lua_tointeger( state, 2);

    l_obj = lua_entitylist->getEntity( l_id);
    if( l_obj == NULL) {
        printf( "lua_getVertexHit obj not found\n");
        return 0;
    }

    bool l_hit = l_obj->getVertexHit( l_vertexid);

    lua_pushboolean ( state, l_hit);

    return 1;
}

static int lua_getPosition( lua_State *state) {
    entity *l_obj;
    int l_id;
    if ( !lua_isnumber( state, 1) ) {
        printf( "lua_getPosition call wrong argument\n");
        return 0;
    }

    l_id = lua_tointeger( state, 1);

    l_obj = lua_entitylist->getEntity( l_id);
    if( l_obj == NULL) {
        printf( "lua_getPosition obj not found\n");
        return 0;
    }
    lua_pushnumber( state, l_obj->getPosition().tovec2().x );
    lua_pushnumber( state, l_obj->getPosition().tovec2().y );

    return 2;
}

static int lua_getHitbox( lua_State *state) {
    entity *l_obj;
    int l_id;
    if ( !lua_isnumber( state, 1) ) {
        printf( "lua_getPosition call wrong argument\n");
        return 0;
    }

    l_id = lua_tointeger( state, 1);

    l_obj = lua_entitylist->getEntity( l_id);
    if( l_obj == NULL) {
        printf( "lua_getPosition obj not found\n");
        return 0;
    }
    lua_pushnumber( state, l_obj->getType()->getHitbox().x );
    lua_pushnumber( state, l_obj->getType()->getHitbox().y );

    return 2;
}

static int lua_getHitboxOffset( lua_State *state) {
    entity *l_obj;
    int l_id;
    if ( !lua_isnumber( state, 1) ) {
        printf( "lua_getPosition call wrong argument\n");
        return 0;
    }

    l_id = lua_tointeger( state, 1);

    l_obj = lua_entitylist->getEntity( l_id);
    if( l_obj == NULL) {
        printf( "lua_getPosition obj not found\n");
        return 0;
    }
    lua_pushnumber( state, l_obj->getType()->getHitboxOffset().x );
    lua_pushnumber( state, l_obj->getType()->getHitboxOffset().y );

    return 2;
}

static int lua_setGravity( lua_State *state) {
    entity *l_obj;
    int l_id;
    bool l_gravity;
    if ( !lua_isnumber( state, 1) || !lua_isboolean( state, 2)) {
        printf( "lua_setGravity call wrong argument\n");
        return 0;
    }

    l_id = lua_tointeger( state, 1);
    l_gravity = lua_toboolean( state, 2);

    l_obj = lua_entitylist->getEntity( l_id);
    if( l_obj == NULL) {
        printf( "lua_setGravity obj not found\n");
        return 0;
    }
    l_obj->setGravity( l_gravity);
    return 0;
}

static int lua_getName( lua_State *state) {
    entity *l_obj;
    int l_id;
    std::string l_name;

    if ( !lua_isnumber( state, 1) ) {
        printf( "lua_getName call wrong argument\n");
        return 0;
    }

    l_id = lua_tointeger( state, 1);

    l_obj = lua_entitylist->getEntity( l_id);
    if( l_obj == NULL) {
        printf( "lua_getName obj not found\n");
        return 0;
    }
    l_name = l_obj->getType()->getName();

    lua_pushstring ( state, l_name.c_str());

    return 1;
}

static int lua_getGravity( lua_State *state) {
    entity *l_obj;
    int l_id;
    bool l_gravity;
    if ( !lua_isnumber( state, 1) ) {
        printf( "lua_getGravity call wrong argument\n");
        return 0;
    }

    l_id = lua_tointeger( state, 1);

    l_obj = lua_entitylist->getEntity( l_id);
    if( l_obj == NULL) {
        printf( "lua_getGravity obj not found\n");
        return 0;
    }
    l_gravity = l_obj->getGravity();

    lua_pushboolean ( state, l_gravity);

    return 1;
}

static int lua_setDepth( lua_State *state) {
    entity *l_obj;
    int l_id;
    int l_depth;
    if ( !lua_isnumber( state, 1) || !lua_isnumber( state, 2)) {
        printf( "lua_setDepth call wrong argument\n");
        return 0;
    }

    l_id = lua_tointeger( state, 1);
    l_depth = lua_tointeger( state, 2);

    l_obj = lua_entitylist->getEntity( l_id);
    if( l_obj == NULL) {
        printf( "lua_setDepth obj not found\n");
        return 0;
    }
    l_obj->setDepth( l_depth);
    return 0;
}


void lua_install( lua_State *state) {
    // add all entity function ..
    lua_pushcfunction( state, lua_print);
    lua_setglobal( state, "print");

    lua_pushcfunction( state, lua_play_sound);
    lua_setglobal( state, "play_sound");

    lua_pushcfunction( state, lua_message);
    lua_setglobal( state, "message");

    lua_pushcfunction( state, lua_setLoadLevel);
    lua_setglobal( state, "setLoadLevel");

    lua_pushcfunction( state, lua_end_level);
    lua_setglobal( state, "end_level");

    lua_pushcfunction( state, lua_getTile);
    lua_setglobal( state, "getTile");

    lua_pushcfunction( state, lua_setSolid);
    lua_setglobal( state, "setSolid");

    lua_pushcfunction( state, lua_setAlpha);
    lua_setglobal( state, "setAlpha");

    lua_pushcfunction( state, lua_sendSignal);
    lua_setglobal( state, "sendSignal");

    lua_pushcfunction( state, lua_howManyPlayerEntity);
    lua_setglobal( state, "howManyPlayerEntity");

    lua_pushcfunction( state, lua_findObjects);
    lua_setglobal( state, "findObjects");

    lua_pushcfunction( state, lua_isEnemy);
    lua_setglobal( state, "isEnemy");

    lua_pushcfunction( state, lua_isPlayer);
    lua_setglobal( state, "isPlayer");

    lua_pushcfunction( state, lua_getAmountPlayerChamps);
    lua_setglobal( state, "getAmountPlayerChamps");

    lua_pushcfunction( state, lua_kill);
    lua_setglobal( state, "kill");

    lua_pushcfunction( state, lua_delete);
    lua_setglobal( state, "delete");

    lua_pushcfunction( state, lua_createObject);
    lua_setglobal( state, "createObject");

    lua_pushcfunction( state, lua_isAlive);
    lua_setglobal( state, "isAlive");

    lua_pushcfunction( state, lua_setAnimation);
    lua_setglobal( state, "setAnimation");

    lua_pushcfunction( state, lua_setAnimationDirection);
    lua_setglobal( state, "setAnimationDirection");

    lua_pushcfunction( state, lua_getAnimation);
    lua_setglobal( state, "getAnimation");

    lua_pushcfunction( state, lua_getAnimationDirection);
    lua_setglobal( state, "getAnimationDirection");

    lua_pushcfunction( state, lua_getFrame);
    lua_setglobal( state, "getFrame");

    lua_pushcfunction( state, lua_setPosition);
    lua_setglobal( state, "setPosition");

    lua_pushcfunction( state, lua_addVelocity);
    lua_setglobal( state, "addVelocity");

    lua_pushcfunction( state, lua_getColision);
    lua_setglobal( state, "getColision");

    lua_pushcfunction( state, lua_setVelocityX);
    lua_setglobal( state, "setVelocityX");

    lua_pushcfunction( state, lua_setVelocityY);
    lua_setglobal( state, "setVelocityY");

    lua_pushcfunction( state, lua_getVelocity);
    lua_setglobal( state, "getVelocity");

    lua_pushcfunction( state, lua_getVertexHit);
    lua_setglobal( state, "getVertexHit");

    lua_pushcfunction( state, lua_getPosition);
    lua_setglobal( state, "getPosition");

    lua_pushcfunction( state, lua_getName);
    lua_setglobal( state, "getName");

    lua_pushcfunction( state, lua_getHitbox);
    lua_setglobal( state, "getHitbox");

    lua_pushcfunction( state, lua_getHitboxOffset);
    lua_setglobal( state, "getHitboxOffset");

    lua_pushcfunction( state, lua_getGravity);
    lua_setglobal( state, "getGravity");

    lua_pushcfunction( state, lua_setGravity);
    lua_setglobal( state, "setGravity");

    lua_pushcfunction( state, lua_setDepth);
    lua_setglobal( state, "setDepth");
}

void lua_setLink( entitylist *entity, world *world, graphic *graphic) {
    // set list
    lua_entitylist = entity;
    lua_worldlist = world;
    lua_graphic = graphic;
}

entitytype::entitytype() {
}

entitytype::~entitytype() {
        // free vector
        p_vertex.clear();

        // free all sounds
        for( auto const &l_sound:p_sound) {
            delete l_sound.sound;
        }

        // free image
        for( auto const &l_action:p_actions) {
            delete l_action.imagefile;
        }

        // finally clear the vector
        p_sound.clear();
}

action* entitytype::getAction( std::string name) {
    for( int i = 0; i < (int)p_actions.size(); i++) {
        if( p_actions[i].name == name)
            return &p_actions[i];
    }

    return NULL;
}

void entitytype::addAction( std::string name, std::string file, int frames, int speed, int loop, image *image, std::string startcall, std::string endcall) {
    action *l_action = new action;

    l_action->name = name;
    l_action->file = file;
    l_action->frames = frames;
    l_action->speed = speed;
    l_action->imagefile = image;
    l_action->loop = loop;
    l_action->startcall = startcall;
    l_action->endcall = endcall;

    p_actions.push_back( *l_action);
}

void entitytype::addVertex(vec2 pos, bool left, bool right, bool up, bool down, int id) {
    vertex *l_vertex = new vertex;

    l_vertex->id = id;
    l_vertex->pos = pos;
    l_vertex->left = left;
    l_vertex->right = right;
    l_vertex->down = down;
    l_vertex->up = up;

    p_vertex.push_back( *l_vertex);
}

void entitytype::addSound( std::string name, std::string file, int volume) {
    entity_sound l_sound;

    // creating sound obj
    l_sound.sound = new sound();

     // load file
    if( l_sound.sound->loadSound( file) == false) {
        // delete sound obj and dont save
        printf( "entitytype::addSound type:\"%s\" load soundfile \"%s\" error: %s\n", p_name.c_str(), file.c_str(), Mix_GetError());
        delete l_sound.sound;
        return;
    }

    // set values
    l_sound.name = name;
    l_sound.volume = volume;

    p_sound.push_back( l_sound);
}

entity::entity( int id)
{
    // set id
    p_id = id;
    p_direction = 0;

    p_state = NULL;

    // timer start
    p_time.start();
    p_timer.start();
    p_timeCall.start();

    // framecounter
    p_timestartaction = 0;

    // frame 0
    p_frame = 0;
    p_depth = p_id;

    // standard alpha
    setAlpha( 255);

    // not in p_liquid
    p_liquid = false;

    // not yet delete
    isbedelete = false;

    p_down = false;
    p_up = false;
    p_right = false;
    p_left = false;
}

entity::~entity()
{
    //if( p_state)
        //lua_close( p_state);
}

void entity::draw( graphic *graphic) {
    int l_frame;

    // graphic update
    if( NeedUpdate())
        lua_update( getId());

    // p_timestartaction
    if( p_timestartaction == -1)
        p_timestartaction = graphic->getFrame()+1;

    if( p_type == NULL) {
        printf("entity::draw \"%s\" dont found p_type!\n", this->getType()->getName().c_str());
        return;
    }

    action *l_action = p_type->getAction( this->p_action);
    if( l_action == NULL) { // falls animaton fehlt zurück zu idle
        printf("entity::draw \"%s\" \"%s\" dont found action, jump to \"idle\"\n", this->getType()->getName().c_str(),this->p_action.c_str());
        setAction( ACTION_IDLE);
        return;
    }

    image *l_image = l_action->imagefile;
    if( l_image == NULL)
        printf("entity::draw no img found %s\n", p_type->getName().c_str());

    // if no frames
    if( l_action->frames == 0)
        l_action->frames = 1;

    int l_action_frame_time = graphic->getFrame()-p_timestartaction;

    l_frame = (int)(l_action_frame_time/l_action->speed);
    if( l_action->speed == 0)
        l_frame = 100; // speed up

    // ende der frames
    bool l_flagEnd = false;
    while( l_frame >= l_action->frames) {
        // loop
        if( l_action->loop) {
            p_timestartaction = graphic->getFrame();
            l_frame--;
        } else {
            l_flagEnd = true;
            l_frame--;
        }
    }

    // look if we have a end call
    if( l_flagEnd && l_action->endcall.size() > 0 && p_flagEndCall == false) {
        // once
        p_flagEndCall = true;
        // call the lua call
        lua_action( l_action->endcall);
    }

    // keine Negative werte
    if( l_frame < 0)
        l_frame = 0;

    // save the frame
    p_frame = l_frame;

    fvec2 l_pos_tmp;

    // HUD move with camera
    if( getType()->getIsHUD())
        l_pos_tmp = p_pos + graphic->getCamera();
    else
        l_pos_tmp = p_pos;

    // alpha
    if( this->getAlpha() != 255)
        l_image->setAlpha( getAlpha());

    // draw call
    graphic->drawImage( l_image, l_pos_tmp.tovec2(), vec2( p_type->getWidth(),p_type->getHeight()), vec2( p_type->getWidth()*l_frame, 0), 0, p_direction);

    if( this->getAlpha() != 255)
        l_image->setAlpha( 255);
}

void entity::setAction( std::string name, bool withStartCall) {

    // check if name is void
    if( p_action != name) {
        p_action = name;
        p_timestartaction = -1;
        p_frame = 0;
        p_flagEndCall = false;

        if( p_type != NULL && withStartCall == true) {
            // get action
            action *l_action = p_type->getAction( this->p_action);
            if( l_action != NULL) {
                //start lua action
                if( l_action->startcall.size() > 0)
                    lua_action( l_action->startcall);
            }
        }
    }
}

void entity::loadScript( std::string file) {
    int l_result;

    if( file.size() < 1) // no file
        return;

    if( file_exist( file) == false) {
        printf( "entitytype::loadScript file '%s' dont found\n", file.c_str());
        return;
    }

    p_state = luaL_newstate();
    if( p_state == NULL) {
        printf( "entity::loadScript cannot create new lua state\n" );
        return;
    }

    // standard libraries
    luaL_openlibs( p_state);

    // load the file
    l_result = luaL_dofile( p_state, file.c_str());
    if( l_result != LUA_OK ) {
        // print error
        lua_printerror();
    }

    // install lua entity function
    lua_install( p_state);
}

void entity::lua_start( int id) {
    if( p_state == NULL)
        return;
    // name the function
    lua_getglobal( p_state, "start");
    if( !lua_isfunction( p_state, -1)) {
        lua_pop( p_state,1);
        return;
    }
    lua_pushnumber( p_state, id);
    // call the function
    if( lua_pcall( p_state, 1, 0, 0))
        printf("entity::lua_start %s\n", lua_tostring( p_state, -1));
}

void entity::lua_vertexhit( int id) {
    if( p_state == NULL)
        return;
    // name the function
    lua_getglobal( p_state, "vertexhit");
    if( !lua_isfunction( p_state, -1)) {
        lua_pop( p_state,1);
        return;
    }
    lua_pushnumber( p_state, id);
    // call the function
    if( lua_pcall( p_state, 1, 0, 0))
        printf("entity::lua_vertexhit %s\n", lua_tostring( p_state, -1));
}

void entity::lua_jump( int id) {
    if( p_state == NULL)
        return;
    // name the function
    lua_getglobal( p_state, "jump");
    if( !lua_isfunction( p_state, -1)) {
        lua_pop( p_state, 1);
        return;
    }
    lua_pushnumber( p_state, id);
    // call the function
    if( lua_pcall( p_state, 1, 0, 0))
        printf("entity::lua_jump %s\n", lua_tostring( p_state, -1));
    //lua_pop( p_state, 1);
}

void entity::lua_right( int id) {
    if( p_state == NULL)
        return;
    // name the function
    lua_getglobal( p_state, "right");
    if( !lua_isfunction( p_state, -1)) {
        lua_pop( p_state,1);
        return;
    }
    lua_pushnumber( p_state, id);
    // call the function
    if( lua_pcall( p_state, 1, 0, 0))
        printf("entity::lua_right %s\n", lua_tostring( p_state, -1));
}

void entity::lua_right_once( int id) {
    // name the function
    lua_getglobal( p_state, "right_once");
    if( !lua_isfunction( p_state, -1)) {
        lua_pop( p_state,1);
        return;
    }
    lua_pushnumber( p_state, id);
    // call the function
    if( lua_pcall( p_state, 1, 0, 0))
        printf("entity::lua_right_once %s\n", lua_tostring( p_state, -1));
}

void entity::lua_left( int id) {
    if( p_state == NULL)
        return;
    // name the function
    lua_getglobal( p_state, "left");
    if( !lua_isfunction( p_state, -1)) {
        lua_pop( p_state,1);
        return;
    }
    lua_pushnumber( p_state, id);
    // call the function
    if( lua_pcall( p_state, 1, 0, 0))
        printf("entity::lua_left %s\n", lua_tostring( p_state, -1));
}

void entity::lua_left_once( int id) {
    // name the function
    lua_getglobal( p_state, "left_once");
    if( !lua_isfunction( p_state, -1)) {
        lua_pop( p_state,1);
        return;
    }
    lua_pushnumber( p_state, id);
    // call the function
    if( lua_pcall( p_state, 1, 0, 0))
        printf("entity::lua_left_once %s\n", lua_tostring( p_state, -1));
}

void entity::lua_up( int id) {
    if( p_state == NULL)
        return;
    // name the function
    lua_getglobal( p_state, "up");
    if( !lua_isfunction( p_state, -1)) {
        lua_pop( p_state,1);
        return;
    }
    lua_pushnumber( p_state, id);
    // call the function
    if( lua_pcall( p_state, 1, 0, 0))
        printf("entity::lua_up %s\n", lua_tostring( p_state, -1));
}

void entity::lua_up_once( int id) {
    // name the function
    lua_getglobal( p_state, "up_once");
    if( !lua_isfunction( p_state, -1)) {
        lua_pop( p_state,1);
        return;
    }
    lua_pushnumber( p_state, id);
    // call the function
    if( lua_pcall( p_state, 1, 0, 0))
        printf("entity::lua_up_once %s\n", lua_tostring( p_state, -1));
}

void entity::lua_down( int id) {
    if( p_state == NULL)
        return;
    // name the function
    lua_getglobal( p_state, "down");
    if( !lua_isfunction( p_state, -1)) {
        lua_pop( p_state,1);
        return;
    }
    lua_pushnumber( p_state, id);
    // call the function
    if( lua_pcall( p_state, 1, 0, 0))
        printf("entity::lua_down %s\n", lua_tostring( p_state, -1));
}

void entity::lua_down_once( int id) {
    // name the function
    lua_getglobal( p_state, "down_once");
    if( !lua_isfunction( p_state, -1)) {
        lua_pop( p_state,1);
        return;
    }
    lua_pushnumber( p_state, id);
    // call the function
    if( lua_pcall( p_state, 1, 0, 0))
        printf("entity::lua_down_once %s\n", lua_tostring( p_state, -1));
}

void entity::lua_run( int id, bool press) {
    if( p_state == NULL)
        return;
    // name the function
    lua_getglobal( p_state, "run");
    if( !lua_isfunction( p_state, -1)) {
        lua_pop( p_state,1);
        return;
    }
    lua_pushnumber( p_state, id);
    lua_pushboolean( p_state, press);
    // call the function
    if( lua_pcall( p_state, 2, 0, 0))
        printf("entity::lua_run %s\n", lua_tostring( p_state, -1));
}

void entity::lua_attack( int id) {
    if( p_state == NULL)
        return;
    // name the function
    lua_getglobal( p_state, "attack");
    if( !lua_isfunction( p_state, -1)) {
        lua_pop( p_state,1);
        return;
    }
    lua_pushnumber( p_state, id);
    // call the function
    if( lua_pcall( p_state, 1, 0, 0))
        printf("entity::lua_attack %s\n", lua_tostring( p_state, -1));
}

void entity::lua_special( int id) {
    if( p_state == NULL)
        return;
    // name the function
    lua_getglobal( p_state, "special");
    if( !lua_isfunction( p_state, -1)) {
        lua_pop( p_state,1);
        return;
    }
    lua_pushnumber( p_state, id);
    // call the function
    if( lua_pcall( p_state, 1, 0, 0))
        printf("entity::lua_special %s\n", lua_tostring( p_state, -1));
}

void entity::lua_update( int id) {
    if( p_state == NULL)
        return;
    // name the function
    lua_getglobal( p_state, "update");
    if( !lua_isfunction( p_state, -1)) {
        lua_pop( p_state,1);
        return;
    }
    lua_pushnumber( p_state, id);
    // call the function
    if( lua_pcall( p_state, 1, 0, 0))
        printf("entity::lua_update %s\n", lua_tostring( p_state, -1));
}

void entity::lua_collision( int id, std::vector<int> ids) {
    if( p_state == NULL || (int)ids.size() == 0)
        return;
    // name the function
    lua_getglobal( p_state, "collision");
    if( !lua_isfunction( p_state, -1)) {
        lua_pop( p_state, 1);
        return;
    }
    lua_pushnumber( p_state, id);
    for( int i = 0; i < (int)ids.size(); i++) {
        lua_pushnumber( p_state, ids[i]);
    }

    // call the function
    if( lua_pcall( p_state, (int)ids.size()+1, 0, 0))
        printf("entity::lua_collision %s\n", lua_tostring( p_state, -1));
}

void entity::lua_liquid( int id) {
    if( p_state == NULL)
        return;
    // name the function
    lua_getglobal( p_state, "liquid");
    if( !lua_isfunction( p_state, -1)) {
        lua_pop( p_state,1);
        return;
    }
    lua_pushnumber( p_state, id);
    lua_pushboolean( p_state, this->isInLiquid());
    // call the function
    if( lua_pcall( p_state, 2, 0, 0))
        printf("entity::lua_liquid %s\n", lua_tostring( p_state, -1));
}

int entity::lua_timer( int id, int time) {
    if( p_state == NULL)
        return 0;
    // name the function
    lua_getglobal( p_state, "timer");
    if( !lua_isfunction( p_state, -1)) {
        lua_pop( p_state,1);
        return 0;
    }
    lua_pushnumber( p_state, id);
    lua_pushnumber( p_state, time);
    // call the function
    if( lua_pcall( p_state, 2, 0, 0))
        printf("entity::lua_timer %s\n", lua_tostring( p_state, -1));
    return time;
}

void entity::lua_timeCall( int id) {
    if( p_state == NULL)
        return;
    // get count number
    int l_time = p_time.getTicks()/1000;// seconds

    // write the function
    char l_function[256]; // long ._.
    sprintf( l_function, "time%d", l_time);

    // search after the function
    lua_getglobal( p_state, l_function);
    if( !lua_isfunction( p_state, -1)) { // not found -> cancel
        lua_pop( p_state,1);
        return;
    }
    // push id
    lua_pushnumber( p_state, id);

    // call the function
    if( lua_pcall( p_state, 1, 0, 0))
        printf("entity::lua_timeCall %s\n", lua_tostring( p_state, -1));
}
void entity::lua_signal( int id, int fromId, std::string data) {
    if( p_state == NULL)
        return;

    // search after the function
    lua_getglobal( p_state, "signal");
    if( !lua_isfunction( p_state, -1)) { // not found -> cancel
        lua_pop( p_state,1);
        return;
    }
    // push id
    lua_pushnumber( p_state, id);
    lua_pushnumber( p_state, fromId);
    lua_pushstring( p_state, data.c_str());

    // call the function
    if( lua_pcall( p_state, 3, 0, 0))
        printf("entity::lua_signal %s\n", lua_tostring( p_state, -1));
}

void entity::lua_printerror() {
  // The error message is on top of the stack.
  // Fetch it, print it and then pop it off the stack.
  const char* message = lua_tostring( p_state, -1);
  puts(message);
  lua_pop( p_state, 1);
}

bool entity::getVertexHit( int id) {
    for( int i = 0; i < (int)p_vertex.size(); i++) {
        if( p_vertex[i].id == id) {
            if( p_vertex[i].hit == true)
                return true;
            else
                return false;
        }
    }
    return false;
}


entitylist::entitylist() {
    // start at 0
    p_id = 1;
}

entitylist::~entitylist() {
    // free first entity
    for(auto const& l_entity: p_entitys)
        delete l_entity;
    p_entitys.clear();

    // now the types!
    for(auto const& l_entity_type: p_entity_types)
        delete l_entity_type;
    p_entity_types.clear();
}

int entitylist::create( entitytype *type, vec2 pos, int id) {
    entity* obj;
    int l_id = id;

    // check data
    if( type == NULL)
        return -1;

    // chose l_id or p_id
    if( l_id == -1)
        l_id = p_id;

    //printf( "creating \"%s\" with id %d\n", type->getName().c_str(), l_id);

    // create object
    obj = new entity( l_id);
    // set data
    obj->setAction( ACTION_IDLE, false);
    obj->setType( type);
    obj->setPos( pos);
    obj->setVertex( type->getVertex());
    obj->setSolid( type->getIsSolid());
    obj->setGravity( type->getGravity());

    // load if script are set
    if( type->getScriptName().size() > 0)
        obj->loadScript( type->getScriptName());

    // add to vector
    p_entitys.push_back( obj);

    // start script if write
    getEntity( l_id)->lua_start( l_id);

    // incress next id
    l_id++;

    if( p_id < l_id)
        p_id = l_id;

    return (l_id-1);
}

void entitylist::deleteObj( int id) {
    for( int i = 0; i < (int)p_entitys.size(); i++)
        if( p_entitys[i]->getId() == id)
            p_entitys.erase( p_entitys.begin()+i);
}

std::vector<int> entitylist::createFromWorldFile( std::string file, world *world) {
    XMLDocument l_file;
    std::string l_type;
    int l_id;
    std::vector<int> l_ids;

    vec2 l_pos;

    // load form world file
    //XMLError l_result =
    l_file.LoadFile( file.c_str());
    // check the file
    //XMLCheckResult(l_result);

    // file exist?
    if( file_exist( file) == false) {
        printf( "entitylist::createFromWorldFile file dont exist \"%s\"\n", file.c_str());
        return l_ids;
    }

    XMLElement* l_map = l_file.FirstChildElement( "map" );
    if( !l_map) {
        printf( "entitylist::createFromWorldFile world has no map defined\n");
        return l_ids;
    }

    XMLElement* l_objectgroup = l_map->FirstChildElement( "objectgroup" );
    if( !l_objectgroup) {
        printf( "entitylist::createFromWorldFile objectgroup has no map defined\n");
        return l_ids;
    }

    XMLElement* l_object = l_objectgroup->FirstChildElement( "object" );
    //
    while( l_object) {

        // load data
        l_type = l_object->Attribute("name") == NULL?"":l_object->Attribute("name");
        l_id = l_object->Attribute("id") == NULL?-1:atoi(l_object->Attribute("id"));

        l_pos.x = atoi(l_object->Attribute( "x"));
        l_pos.y = atoi(l_object->Attribute( "y"));

        //printf("%s %d %d\n", l_type.c_str(), l_pos.x, l_pos.y);

        //
        entitytype *l_entity_type = getType( l_type);

        if( l_entity_type != NULL) {
            l_id = create( l_entity_type, l_pos, l_id);
            entity *l_entity = getEntity( l_id);

            // sammel alle ids
            l_ids.push_back( l_id);

            // random start animation
            l_entity->setTimeStartAction( rand()%20);

            // load properties
            XMLElement* l_xml_property = NULL;
            XMLElement* l_xml_properties = l_object->FirstChildElement( "properties" );

            // if properties write
            if( l_xml_properties != NULL)
                l_xml_property = l_xml_properties->FirstChildElement( "property" );
            while( l_xml_property) {
                std::string l_property = l_xml_property->Attribute("name") == NULL?"":l_xml_property->Attribute("name");
                std::string l_value = l_xml_property->Attribute("value") == NULL?"":l_xml_property->Attribute("value");

                //  handle property
                if( l_property == "dir" && l_value == "left")
                    l_entity->setDirection( true);
                else if( l_property == "action")
                    l_entity->setAction( l_value);
                else if( l_property == "global")
                    l_entity->setGlobal( l_value);
                else if( l_property == "global2")
                    l_entity->setGlobal2( l_value);
                else if( l_property == "global3")
                    l_entity->setGlobal3( l_value);

                l_xml_property = l_xml_property->NextSiblingElement( "property");
            }
        } else if( l_type == "start"){ // wolrd start point
            world->setStartPoint( l_pos);
        }

        // next object
        l_object = l_object->NextSiblingElement( "object");
    }

    return l_ids;
}

void entitylist::draw(graphic *graphic, particle_list* particle, config *config, bool depth) {
    // depth
    if( depth == true )
        std::sort( p_entitys.begin(), p_entitys.end());

    for(int i = 0; i < (int)p_entitys.size(); i++) {
        entity *l_obj = p_entitys[i];
        // if not NULL
        if( l_obj != NULL)
            // HUD element skip
            if( l_obj->getType()->getIsHUD())
                continue;

            // draw the entity
            l_obj->draw( graphic);
            // debug
            if( config->get( "debug", "game") == "true" ) {
                SDL_Rect rect = {   l_obj->getPosition().tovec2().x + l_obj->getType()->getHitboxOffset().x,
                                    l_obj->getPosition().tovec2().y + l_obj->getType()->getHitboxOffset().y,
                                    l_obj->getType()->getHitbox().x,
                                    l_obj->getType()->getHitbox().y};
                rect.x -= graphic->getCamera().x;
                rect.y -= graphic->getCamera().y;

                SDL_SetRenderDrawColor( graphic->getRenderer(), 255, 0, 0, 255);
                // only with hitbox show
                if( l_obj->getType()->getHitbox().x != 0)
                    SDL_RenderDrawRect( graphic->getRenderer(), &rect );

                // draw all vertex
                for( int n = 0; n < (int)l_obj->getVertex()->size(); n++) {
                    int l_blue = 0;
                    vertex *l_vertex = &l_obj->getVertex()->at(n);
                    fvec2 l_position = l_obj->getPosition();
                    vec2 l_collision_pos = l_position.tovec2() + l_vertex->pos - graphic->getCamera().tovec2();

                    // if hit other color
                    if( l_vertex->hit == true)
                        l_blue = 255;

                    // draw the point
                    SDL_SetRenderDrawColor( graphic->getRenderer(), l_blue, 255, l_blue, 255);
                    SDL_RenderDrawPoint( graphic->getRenderer(), l_collision_pos.x, l_collision_pos.y);
                }
            }
    }

    // draw text
    for( int i = 0; i < (int)p_text.size(); i++) {
        entity_text *l_text = p_text[i];
        entity *l_obj = getEntity( l_text->id);
        // if object found add particle
        if( l_obj) {
            // anzeigen
            particle->createParticel( par_text, l_obj->getPosition() + l_text->offset, l_obj->getVelocity(), l_text->lifetime, l_text->text, fvec2( l_text->size, l_text->size), l_text->asHUD, false);
        }
    }
    // aufräumen falls was drin ist
    if( (int)p_text.size())
        p_text.clear();
}

void entitylist::drawHUD( graphic *graphic) {
    for(int i = 0; i < (int)p_entitys.size(); i++) {
        entity *l_obj = p_entitys[i];
        // if not NULL
        if( l_obj != NULL)
            // only HUD elements
            if( l_obj->getType()->getIsHUD() == false)
                continue;

            // draw the entity
            l_obj->draw( graphic);
    }
    return;
}

int entitylist::setVertexHit( vertex *vertex, bool set){
    bool l_old;

    l_old = vertex->hit;
    vertex->setHit( set);

    if( l_old != set)
        return 1;
    return 0;
}

void entitylist::process( world *world, config *config, int deltaTime) {
    float l_velocityDelta;
    int l_vertexhitchange;
    vec2 l_iposition;

    // calc delta of velocity
    l_velocityDelta = (float)deltaTime * world->getGravity();

    // again and again if we found o+ne more to delete we search again
    for(int i = 0; i < (int)p_entitys.size(); i++) {
        if( p_entitys[i]->isbedelete == true ) {
            p_entitys.erase( p_entitys.begin() + i);
            i--;
        }
    }

    // go through
    for(int i = 0; i < (int)p_entitys.size(); i++) {
        entity *l_entity = p_entitys[i];

        // skip if entity null -> it cant be happen but
        if( l_entity == NULL) {
            printf( "entitylist::process l_entity == NULL - fataler fehler\n");
            continue;
        }

        // deleting flag is set, no calculation
        if( l_entity->isbedelete == true )
            continue;

        // get type
        entitytype *l_type = l_entity->getType();

        // delete obj if he go out
        if( l_entity->getPosition().y > world->getWorld().y*world->getTileSize().y) {
            deleteObj( l_entity->getId());
            continue;
        }

        // lua timer for simple ai or all other timing aspects
        if( l_type->getTimerTime() > 0) {
            // time over
            if( l_entity->getTimer()->getTicks() > l_type->getTimerTime() ) {
                // start again
                l_entity->getTimer()->start();
                l_entity->lua_timer( l_entity->getId(), l_type->getTimerTime());
            }
        }

        // timeCall
        if( l_type->getHasTimeCall() == true && l_entity->getTimeCall()->getTicks() > 1000) {
            // reset counter
            l_entity->resetCallTime();

            l_entity->lua_timeCall( l_entity->getId());
        }

        // liquid
        if( l_entity->getGravity() == true && l_type->getHitboxOffset().x != 0) {
            // umrechnen
            l_iposition = (l_entity->getPosition().tovec2()+l_type->getHitboxOffset().tovec2()+l_type->getHitbox().tovec2()/vec2( 2, 2 ) + world->getTileSize()/vec2( 2, 2 )-(world->getTileSize()/vec2( 2, 2) ) )/world->getTileSize();
            tile *l_tile = world->getTile( world->getCollsionMap(), l_iposition);

            if( l_tile != NULL && l_tile->type != NULL && l_tile->type->liquid == true ) {
                // only once
                if(l_entity->isInLiquid() == false)
                    l_entity->setLiquid( true); // he swim
            } else if(l_entity->isInLiquid() == true)
                l_entity->setLiquid( false); // he now not swim anymore
        }

        // check about the hitting box
        std::vector<int> l_ids;
        l_ids = collision_boundingBox( l_entity);
        if( l_ids.size() > 0)
            l_entity->lua_collision( l_entity->getId(), l_ids);
        std::vector<int> l_ids_vertex = collision_boundingBoxVertex( l_entity);
        // id add vertex id
        l_ids.insert( l_ids.end(), l_ids_vertex.begin(), l_ids_vertex.end() );

        fvec2 l_velocity;
        fvec2 l_position;
        fvec2 l_change;

        // positon ermiteln
        l_position = l_entity->getPosition();
        l_velocity = l_entity->getVelocity();

        // calc gravity
        bool l_iscalc_y = false;

        // add gravity
        if( l_entity->getGravity() == true && l_type->getIsTopView() == false) {
            // änderung rechnen
            l_change.x += l_velocity.x * deltaTime;

            if( l_velocityDelta ) {
                float l_delta_factor = (( l_velocity.y + (l_velocityDelta / 2.f)) * deltaTime);

                if(l_entity->isInLiquid() == true)
                    l_delta_factor = l_delta_factor/10.f;

                l_change.y += l_delta_factor;
            }

            // y delta dazurechnen (x nicht nötig da keine gravi. )
            l_velocity.y += l_velocityDelta;
        } else {
            l_change.x += l_velocity.x * deltaTime;
            l_change.y += l_velocity.y * deltaTime;
        }

        // reset collision
        l_entity->setColisionDown( false);
        l_entity->setColisionUp( false);
        l_entity->setColisionLeft( false);
        l_entity->setColisionRight( false);

        fvec2 l_result_change = l_change;
        fvec2 l_result_velocity = l_velocity;

        // vertex change set to 0
        l_vertexhitchange = 0;

        for( int n = 0; n < (int)l_entity->getVertex()->size(); n++) {
            vertex *l_vertex = &l_entity->getVertex()->at(n);
            fvec2 l_collision_pos = l_position + l_vertex->pos;

            l_vertex->hit = false;
            bool l_changed = false;

            // collison check ground
            if( l_vertex->down ) {
                tile *l_tile = NULL;
                l_tile = world->getCollisionTileY( l_collision_pos, l_change, l_velocity);

                if( l_tile != NULL) {
                    // zwischen rechnung
                    float l_pos_change_y = l_collision_pos.y + l_change.y;
                    float l_bottom = (l_tile->pos.y)*world->getTileSize().y;

                    // ausrechnung der änderung
                    float l_result = l_pos_change_y-l_bottom;

                    if( l_result > l_velocity.y && (l_change.y+0.1f)/l_velocity.y > l_result) {
                        if( l_changed == false ) {
                            l_changed = true; // we changed
                            // result
                            l_result_change = l_change - fvec2( 0, l_result);
                            l_result_velocity.y = 0;
                        }


                        l_vertexhitchange += setVertexHit( l_vertex, true);
                        l_entity->setColisionDown( true);
                    }
                }

                // object collision
                if( l_ids.size() > 0 && l_change.y > 0 ) {
                    for( int i = 0; i < (int)l_ids.size(); i++) {
                        entity *l_obj = getEntity( l_ids[i]);
                        // if is solid
                        if( l_obj->isSolid()) {
                            // zwischen rechnung
                            float l_pos_change_y = l_collision_pos.y + l_change.y;
                            float l_bottom = l_obj->getPosition().y + l_obj->getType()->getHitboxOffset().y;

                            float l_result = l_pos_change_y-l_bottom;

                            if( l_result > l_velocity.y && (l_change.y+0.1f)/l_velocity.y > l_result) {
                                if( l_changed == false ) {
                                    l_changed = true; // we change
                                    // result set
                                    l_result_change = l_change - fvec2( 0, l_result);
                                    l_result_velocity.y = l_obj->getVelocity().y>0.0f?l_obj->getVelocity().y:0;

                                    // x achse
                                    l_result_change.x += l_obj->getVelocity().x*(deltaTime);
                                    l_iscalc_y = true;
                                }

                                l_vertexhitchange += setVertexHit( l_vertex, true);
                                l_entity->setColisionDown( true);
                            }
                        }
                    }
                } // object collision
            } // vertex down
            // collison top check
            if( l_vertex->up) {
                tile *l_tile = NULL;
                l_tile = world->getCollisionTileY( l_collision_pos, l_change, l_velocity);

                if( l_tile ) {
                    // zwischen rechnung
                    float l_pos_change_y = l_collision_pos.y + l_change.y;
                    l_pos_change_y -= world->getTileSize().y;
                    float l_bottom = (l_tile->pos.y)*world->getTileSize().y;

                    // ausrechnung der änderung
                    float l_result = l_pos_change_y-l_bottom;

                    if( l_result < l_velocity.y && l_result+1.f > l_change.y*2) {
                        if( l_changed == false ) {
                            l_changed = true; // we changed
                            // result
                            l_result_change = l_change - fvec2( 0, l_result);
                            l_result_velocity.y = 0;
                        }

                        l_vertexhitchange += setVertexHit( l_vertex, true);
                        l_entity->setColisionUp( true);
                    }
                }
                // object collision
                if( l_ids.size() > 0 && l_change.y < 0) {
                    for( int i = 0; i < (int)l_ids.size(); i++) {
                        entity *l_obj = getEntity( l_ids[i]);
                        if( l_obj->isSolid()) {

                          // zwischen rechnung
                            float l_pos_change_y = l_collision_pos.y + l_change.y;
                            float l_bottom = l_obj->getPosition().y + l_obj->getType()->getHitboxOffset().y+l_obj->getType()->getHitbox().y;

                            float l_result = l_pos_change_y-l_bottom;

                            if( l_result < l_velocity.y && l_result+1.f > l_change.y*2) {
                                if( l_changed == false ) {
                                    l_changed = true; // we changed
                                    l_result_change = l_change - fvec2( 0, l_result);

                                    //l_change.y = 0;//l_entity->( fvec2());
                                    l_result_velocity.y = l_obj->getVelocity().y>0.0f?l_obj->getVelocity().y:0;

                                    l_iscalc_y = true;
                                }

                                l_vertexhitchange += setVertexHit( l_vertex, true);
                                l_entity->setColisionUp( true);
                            }
                        }
                    }
                }
            }
        } // ent vertex 1

        // set new wert
        l_change = l_result_change;
        l_velocity = l_result_velocity;

        for( int n = 0; n < (int)l_entity->getVertex()->size(); n++) {
            vertex *l_vertex = &l_entity->getVertex()->at(n);
            fvec2 l_collision_pos = l_position + l_vertex->pos;

            bool l_changed = false;

            if( l_vertex->left) {
                tile *l_tile = NULL;
                l_tile = world->getCollisionTileX( l_collision_pos, l_change, l_velocity);

                if( l_tile ) {
                    // zwischen rechnung
                    float l_pos_change_x = l_collision_pos.x + l_change.x;
                    l_pos_change_x -= world->getTileSize().x;
                    float l_bottom = (l_tile->pos.x)*world->getTileSize().x;

                    // ausrechnung der änderung
                    float l_result = l_pos_change_x-l_bottom;

                    if( l_result < l_velocity.x && l_result+1.f > l_change.x*2) {
                        if( l_changed == false ) {
                            l_changed = true; // we changed
                            // result
                            l_result_change = l_change - fvec2( l_result, 0);
                            l_result_velocity.x = 0;
                        }

                        l_vertexhitchange += setVertexHit( l_vertex, true);
                        l_entity->setColisionLeft( true);
                    }
                }

                // map border left
                if( l_collision_pos.x < 0 && l_velocity.x < 0) {
                    // calc the change
                    l_result_change = l_change - fvec2( l_change.x, 0);
                    l_result_velocity.x = 0;
                    // set left hit
                    l_vertexhitchange += setVertexHit( l_vertex, true);
                    l_entity->setColisionLeft( true);
                }

                // object collision
                if( l_ids.size() > 0 && l_change.x < 0) {
                    for( int i = 0; i < (int)l_ids.size(); i++) {
                        entity *l_obj = getEntity( l_ids[i]);
                        if( l_obj->isSolid()) {
                            // zwischen rechnung
                            float l_pos_change_x = l_collision_pos.x + l_change.x;
                            float l_bottom = l_obj->getPosition().x + l_obj->getType()->getHitboxOffset().x + l_obj->getType()->getHitbox().x;

                            // ausrechnung der änderung
                            float l_result = l_pos_change_x-l_bottom;

                            if( l_result < l_velocity.x && l_result+1.f > l_change.x*2 && l_iscalc_y == false) {
                                if( l_changed == false ) {
                                    l_changed = true;// we changed
                                    // result
                                    l_result_change = l_change - fvec2( l_result, 0);
                                    l_result_velocity.x = 0;
                                }

                                l_vertexhitchange += setVertexHit( l_vertex, true);
                                l_entity->setColisionLeft( true);

                            }
                        }
                    }
                }
            } // vertex left

            if( l_vertex->right) {
                tile *l_tile = NULL;
                l_tile = world->getCollisionTileX( l_collision_pos, l_change, l_velocity);

                if( l_tile ) {
                    // zwischen rechnung
                    float l_pos_change_x = l_collision_pos.x + l_change.x;
                    float l_bottom = (l_tile->pos.x)*world->getTileSize().x;

                    // ausrechnung der änderung
                    float l_result = l_pos_change_x-l_bottom;

                    if( l_result > l_velocity.x && l_result-1.f < l_change.x*2) {
                        if( l_changed == false ) {
                            l_changed = true; // we changed
                            // result
                            l_result_change = l_change - fvec2( l_result, 0);
                            l_result_velocity.x = 0;
                        }

                        l_vertexhitchange += setVertexHit( l_vertex, true);
                        l_entity->setColisionRight( true);
                    }
                }

                // map border
                if( l_collision_pos.x > world->getWorld().x*world->getTileSize().x && l_velocity.x > 0) {
                    l_result_change = l_change - fvec2( l_change.x, 0);

                    //l_change.y = 0;//l_entity->( fvec2());
                    l_result_velocity.x = 0;

                    l_vertexhitchange += setVertexHit( l_vertex, true);
                    l_entity->setColisionRight( true);
                }

                // object collision
                if(  l_ids.size() > 0 && l_change.x > 0) {
                    for( int i = 0; i < (int)l_ids.size(); i++) {
                        entity *l_obj = getEntity( l_ids[i]);
                        if( l_obj->isSolid()) {
                          // zwischen rechnung
                            float l_pos_change_x = l_collision_pos.x + l_change.x;
                            float l_bottom = l_obj->getPosition().x + l_obj->getType()->getHitboxOffset().x;

                            // ausrechnung der änderung
                            float l_result = l_pos_change_x-l_bottom;

                            if( l_result > l_velocity.x && l_result-1.f < l_change.x*2 && l_iscalc_y == false) {
                                if( l_changed == false ) {
                                    l_changed = true; // we changed
                                    // result
                                    l_result_change = l_change - fvec2( l_result, 0);
                                    l_result_velocity.x = 0;
                                }

                                l_vertexhitchange += setVertexHit( l_vertex, true);
                                l_entity->setColisionRight( true);

                            }
                        }
                    }
                } // object collision
            } // vertex right
        } // end vertex 2

        // set new wert
        l_change = l_result_change;
        l_velocity = l_result_velocity;


        // calc gravity
        if( l_type->getIsTopView()) { // top view
            if(l_entity->isInLiquid() == true) {
                l_velocity.x = l_velocity.x*0.5f;
                l_velocity.y = l_velocity.y*0.5f;
            } else {
                l_velocity.x = l_velocity.x*0.8f;
                l_velocity.y = l_velocity.y*0.8f;
            }
        } else if( l_entity->getGravity() == true) { // check if grafity
            if( l_entity->getColisionDown() && !l_entity->isInLiquid())
                l_velocity.x = l_velocity.x*0.8f;
            else if(l_entity->isInLiquid() == true)
                l_velocity.x = l_velocity.x*0.5f;
            else
                l_velocity.x = l_velocity.x*0.99f;
        }

        // calculate depth
        //l_entity->setDepth( (l_entity->getPosition().tovec2().y+l_type->getHitbox().y+l_type->getHitboxOffset().y)*10 +  l_entity->getPosition().x );

        // set net position
        l_entity->setPos( l_position + l_change );

        // add velocity next frame
        l_entity->setVelocity( l_velocity);

        if( l_vertexhitchange > 0) {
            l_entity->lua_vertexhit( l_entity->getId());
        }
    }
}

std::vector <int> entitylist::collision_boundingBox( entity* checkentity) {
    entitytype *l_type = checkentity->getType();
    std::vector <int> l_id;

    fvec2 l_rect1 = fvec2(l_type->getHitboxOffset()) + checkentity->getPosition();
    fvec2 l_rect1_size = l_type->getHitbox();

    int l_entity_id = checkentity->getId();

    //
    for( int i = 0; i < (int)p_entitys.size(); i++)  {
        entity* l_obj = p_entitys[i];
        entitytype *l_typeobj = l_obj->getType();

        // self dont regist
        if( l_obj->getId() == l_entity_id)
            continue;

        // calc rect 2
        fvec2 l_rect2 = fvec2( l_typeobj->getHitboxOffset().x, l_typeobj->getHitboxOffset().y) + l_obj->getPosition();
        fvec2 l_rect2_size = l_typeobj->getHitbox();

        // look if the obj hit the hitbox
        if ( l_rect1.x < l_rect2.x + l_rect2_size.x &&
            l_rect1.x + l_rect1_size.x > l_rect2.x &&
            l_rect1.y < l_rect2.y + l_rect2_size.y &&
            l_rect1_size.y + l_rect1.y > l_rect2.y) {
                l_id.push_back( l_obj->getId());
            }
    }
    return l_id;
}

std::vector <int> entitylist::collision_boundingBoxVertex( entity* checkentity) {
    entitytype *l_type = checkentity->getType();
    std::vector <int> l_id;

    for( int n = 0; n < (int)checkentity->getVertex()->size(); n++) {
        vertex *l_vertex = &checkentity->getVertex()->at(n);

        fvec2 l_rect1 = checkentity->getPosition() + l_vertex->pos;
        fvec2 l_rect1_size = fvec2( 1, 1);

        //
        for( int i = 0; i < (int)p_entitys.size(); i++)  {
            entity* l_obj = p_entitys[i];
            entitytype *l_typeobj = l_obj->getType();

            // self dont regist
            if( l_obj->getId() == checkentity->getId())
                continue;

            // calc rect 2
            fvec2 l_rect2 = l_typeobj->getHitboxOffset() + l_obj->getPosition();
            fvec2 l_rect2_size = l_typeobj->getHitbox();

            // look if the obj hit the hitbox
            if ( l_rect1.x < l_rect2.x + l_rect2_size.x &&
                l_rect1.x + l_rect1_size.x > l_rect2.x &&
                l_rect1.y < l_rect2.y + l_rect2_size.y &&
                l_rect1_size.y + l_rect1.y > l_rect2.y) {
                    l_id.push_back( l_obj->getId());
                }
        }
    }
    return l_id;
}

std::vector <int> entitylist::collision_boundingBoxRect( fvec2 l_postion, fvec2 l_size, int id) {
    std::vector <int> l_id;

    fvec2 l_rect1 = l_postion;
    fvec2 l_rect1_size = l_size;
    //
    for( int i = 0; i < (int)p_entitys.size(); i++)  {
        entity* l_obj = p_entitys[i];
        entitytype *l_typeobj = l_obj->getType();

        // self dont regist
        if( l_obj->getId() == id)
            continue;

        // calc rect 2
        fvec2 l_rect2 = l_typeobj->getHitboxOffset() + l_obj->getPosition().tovec2();
        fvec2 l_rect2_size = l_typeobj->getHitbox();

        // look if the obj hit the hitbox
        if ( l_rect1.x < l_rect2.x + l_rect2_size.x &&
            l_rect1.x + l_rect1_size.x > l_rect2.x &&
            l_rect1.y < l_rect2.y + l_rect2_size.y &&
            l_rect1_size.y + l_rect1.y > l_rect2.y) {
                l_id.push_back( l_obj->getId());
            }
    }
    return l_id;
}

bool entitylist::loadType( std::string folder, graphic *graphic) {
    XMLDocument l_file;
    bool l_idle = false;

    p_folder = folder;

    std::string l_pathfile = folder + (char*)ENTITY_FILE;

    int l_width = 0;
    int l_height = 0;
    bool l_gravity = true;
    bool l_isplayer = false;
    int l_timer = 0;
    bool l_solid = 0;
    bool l_isEnemy = 0;
    bool l_isTopView = false;
    bool l_isHUD = false;
    bool l_hasTimeCall = false;
    std::string l_script;

    std::string l_name;

    // if file dont exist - dont load
    if( file_exist( l_pathfile) == false) {
        return false;
    }

    // load the file
    XMLError l_result = l_file.LoadFile( l_pathfile.c_str());

    // check the file
    XMLCheckResult(l_result);

    //
    XMLElement* l_object = l_file.FirstChildElement( "object" );
    if( l_object == nullptr)
        return false;;

    if( l_object->Attribute( "name"))
        l_name = l_object->Attribute( "name");
    if( l_object->Attribute( "width"))
        l_width = atoi(l_object->Attribute( "width" ));
    if( l_object->Attribute( "height"))
        l_height = atoi(l_object->Attribute( "height"));
    if( l_object->Attribute( "gravity"))
        l_gravity = atoi(l_object->Attribute( "gravity" ));
    if( l_object->Attribute( "player"))
        l_isplayer = atoi(l_object->Attribute( "player" ));
    if( l_object->Attribute( "timer"))
        l_timer = atoi( l_object->Attribute( "timer" ));
    if( l_object->Attribute( "script"))
        l_script = folder + l_object->Attribute( "script");
    if( l_object->Attribute( "solid"))
        l_solid = atoi(l_object->Attribute( "solid" ));
    if( l_object->Attribute( "isenemy"))
        l_isEnemy = atoi(l_object->Attribute( "isenemy" ));
    if( l_object->Attribute( "isTopView"))
        l_isTopView = atoi(l_object->Attribute( "isTopView" ));
    if( l_object->Attribute( "isHUD"))
        l_isHUD = atoi(l_object->Attribute( "isHUD" ));
    if( l_object->Attribute( "hasTimeCall"))
        l_hasTimeCall = atoi(l_object->Attribute( "hasTimeCall" ));

    entitytype *l_type = new entitytype();
    std::string l_action_name;
    std::string l_action_file;
    int l_action_frames;
    int l_action_speed;
    int l_action_loop;
    std::string l_startCall;
    std::string l_endCall;

    // set values
    l_type->setWidth(l_width);
    l_type->setHeight(l_height);
    l_type->setName( l_name);
    l_type->setGravity( l_gravity);
    l_type->setIsPlayer( l_isplayer);
    l_type->setScriptName( l_script);
    l_type->setSolid( l_solid);
    l_type->setTimer( l_timer);
    l_type->setIsEnemy( l_isEnemy);
    l_type->setIsTopView( l_isTopView);
    l_type->setIsHUD( l_isHUD);
    l_type->setHasTimeCall( l_hasTimeCall);

    // read all actions
    XMLElement* l_xml_action = l_object->FirstChildElement( "action" );
    while( l_xml_action) {
        l_action_file = l_xml_action->GetText();
        l_action_name = l_xml_action->Attribute( "name" );
        if( l_xml_action->Attribute( "frame" ))
            l_action_frames = atoi( l_xml_action->Attribute( "frame" ));
        else
            l_action_frames = 1; // default
        if( l_xml_action->Attribute( "speed" ))
            l_action_speed = atoi( l_xml_action->Attribute( "speed" ));
        else
            l_action_speed = 0;

        if( l_xml_action->Attribute( "loop" ))
            l_action_loop = atoi( l_xml_action->Attribute( "loop" ));
        else
            l_action_loop = 1;


        if( l_xml_action->Attribute( "startCall" ))
            l_startCall = l_xml_action->Attribute( "startCall" );
        else
            l_startCall = "";

        if( l_xml_action->Attribute( "endCall" ))
            l_endCall = l_xml_action->Attribute( "endCall" );
        else
            l_endCall = "";

        // check data
        if( l_action_speed == 0)
            l_action_speed = 1; // verry fast animation

        if( l_action_name == ACTION_IDLE)
            l_idle = true;

        // push back
        l_type->addAction( l_action_name, l_action_file, l_action_frames, l_action_speed, l_action_loop,graphic->loadImage( folder + l_action_file), l_startCall, l_endCall);

        l_xml_action = l_xml_action->NextSiblingElement("action");
    }



    // load vertex
    std::string l_vertex_name;
    vec2 l_vertex_pos;
    bool l_vertex_up;
    bool l_vertex_left;
    bool l_vertex_down;
    bool l_vertex_right;
    int l_vertex_id;
    XMLElement* l_xml_vertex = l_object->FirstChildElement( "vertex" );
    while( l_xml_vertex) {
        l_vertex_name = l_xml_vertex->GetText();

        if( l_xml_vertex->Attribute( "id"))
            l_vertex_id = atoi( l_xml_vertex->Attribute( "id"));
        else
            l_vertex_id = 0;

        // reset
        l_vertex_left = false;
        l_vertex_up = false;
        l_vertex_down = false;
        l_vertex_right = false;

        if( l_vertex_name == "up")
            l_vertex_up = true;
        if( l_vertex_name == "left")
            l_vertex_left = true;
        if( l_vertex_name == "down")
            l_vertex_down = true;
        if( l_vertex_name == "right")
            l_vertex_right = true;

        // vertex
        l_vertex_pos.x = atoi(l_xml_vertex->Attribute( "x") );
        l_vertex_pos.y = atoi(l_xml_vertex->Attribute( "y") );

        //printf( "vertex u%dl%dd%dr%d %d/%d\n", l_vertex_up, l_vertex_left, l_vertex_down, l_vertex_right,l_vertex_pos.x, l_vertex_pos.y);
        l_type->addVertex( l_vertex_pos, l_vertex_left, l_vertex_right, l_vertex_up, l_vertex_down, l_vertex_id);

        // next vertex
        l_xml_vertex = l_xml_vertex->NextSiblingElement("vertex");
    }

    vec2 l_hitbox_offset;
    vec2 l_hitbox_size;
    XMLElement* l_xml_hitbox = l_object->FirstChildElement( "hitbox" );
    if( l_xml_hitbox ) {
        // offset
        if( l_xml_hitbox->Attribute( "offsetx"))
            l_hitbox_offset.x = atoi( l_xml_hitbox->Attribute( "offsetx"));
        else
            l_hitbox_offset.x = 0;
        if( l_xml_hitbox->Attribute( "offsety"))
            l_hitbox_offset.y = atoi( l_xml_hitbox->Attribute( "offsety"));
        else
            l_hitbox_offset.y = 0;

        // vertex
        l_hitbox_size.x = atoi(l_xml_hitbox->Attribute( "width") );
        l_hitbox_size.y = atoi(l_xml_hitbox->Attribute( "hight") );

        //printf( "vertex u%dl%dd%dr%d %d/%d\n", l_vertex_up, l_vertex_left, l_vertex_down, l_vertex_right,l_vertex_pos.x, l_vertex_pos.y);
        l_type->setHitbox( l_hitbox_offset, l_hitbox_size);
    }

    // sound items load
    XMLElement* l_xml_sound = l_object->FirstChildElement( "sound" );
    while( l_xml_sound) {
        std::string l_name;
        std::string l_file;
        int l_volume = 100;

        // get file
        l_file = l_xml_sound->GetText();

        // standard volume
        if( l_xml_sound->Attribute( "volume"))
            l_volume = atoi( l_xml_sound->Attribute( "volume"));

        // sound name
        if( l_xml_sound->Attribute( "name"))
            l_name = l_xml_sound->Attribute( "name");

        // load type
        l_type->addSound( l_name, folder + l_file, l_volume);

        // next vertex
        l_xml_sound = l_xml_sound->NextSiblingElement("sound");
    }


    if( !l_idle) {
        printf("entitylist::loadType type %s has no idle action!\n", l_name.c_str());
        return false;
    }

    p_entity_types.push_back( l_type);

    return true;
}

void entitylist::loadTypes( std::string folder, graphic *graphic) {
    DIR *l_dir;

    struct dirent *l_entry;

    std::string l_path = folder; //"creature/";

    l_dir = opendir(l_path.c_str());
    if ( l_dir == NULL) {  /* error opening the directory? */
        printf("game::loadTypes cant load types, dir not found\n");
    }

    while ((l_entry = readdir(l_dir)) != NULL) {
        std::string l_file = l_path + l_entry->d_name + "/";

        // load folder
        loadType( l_file, graphic);

    }
    closedir(l_dir);
}

entitytype* entitylist::getType( std::string name) {
    for( int i = 0; i < (int)p_entity_types.size(); i++)
        if( p_entity_types[i]->getName() == name)
            return p_entity_types[i];
    return NULL;
}

std::vector<int> entitylist::findPlayerObject() {
    std::vector<int> l_obj;

    for( int i = 0; i < (int)p_entitys.size(); i++) {
        entity *l_entity = p_entitys[i];
        if( l_entity->getType()->getIsPlayer() && l_entity->getAction() != "die")
            l_obj.push_back( l_entity->getId());
    }
    return l_obj;
}

void entitylist::message( int id, std::string text, float size, vec2 offset, bool asHUD, int lifetime) {
    entity_text *l_text = new entity_text;

    l_text->id = id;
    l_text->text = text;
    l_text->size = size;
    l_text->offset = offset;
    l_text->asHUD = asHUD,
    l_text->lifetime = lifetime;

    p_text.push_back( l_text);
}

entity* entitylist::getEntity( int id) {
    for( int i = 0; i < (int)p_entitys.size(); i++)
        if( p_entitys[i]->getId() == id)
            return p_entitys[i];
    return NULL;
}

