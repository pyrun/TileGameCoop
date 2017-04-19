#include "entity.h"

#include <dirent.h>
using namespace tinyxml2;

#ifndef XMLCheckResult
	#define XMLCheckResult(a_eResult) if (a_eResult != XML_SUCCESS) { printf("Error: %i\n", a_eResult); return a_eResult; }
#endif

/** LUA FUNCTION */

entitylist *lua_entitylist = NULL;
world *lua_worldlist = NULL;

static int lua_setLoadLevel( lua_State *state) {
    std::string l_level;
    bool l_asPlayer;

    if( !lua_isstring( state, 1) || !lua_isboolean( state, 2) ) {
        printf( "lua_setLoadLevel call wrong argument\n");
        return 0;
    }

    l_level = lua_tostring( state, 1);
    l_asPlayer = lua_toboolean( state, 2);


    lua_worldlist->setLoadWorld( l_level, l_asPlayer);
    return 0;
}

static int lua_end_level( lua_State *state) {
    //bool l_asPlayer;

    if( !lua_isboolean( state, 1) ) {
        printf( "lua_end_level call wrong argument\n");
        return 0;
    }

    //l_asPlayer = lua_toboolean( state, 1);

    lua_worldlist->setEndLevel( true);
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
    int l_id;

    if( !lua_isnumber( state, 1) ) {
        printf( "lua_delete call wrong argument\n");
        return 0;
    }

    l_id = lua_tointeger( state, 1);

    lua_entitylist->deleteObj( l_id);
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

void lua_install( lua_State *state) {
    // add all entity function ..
    lua_pushcfunction( state, lua_setLoadLevel);
    lua_setglobal( state, "setLoadLevel");

    lua_pushcfunction( state, lua_end_level);
    lua_setglobal( state, "end_level");

    lua_pushcfunction( state, lua_getTile);
    lua_setglobal( state, "getTile");

    lua_pushcfunction( state, lua_setSolid);
    lua_setglobal( state, "setSolid");

    lua_pushcfunction( state, lua_isEnemy);
    lua_setglobal( state, "isEnemy");

    lua_pushcfunction( state, lua_isPlayer);
    lua_setglobal( state, "isPlayer");

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

}

void lua_setLink( entitylist *entity, world *world) {
    // set list
    lua_entitylist = entity;
    lua_worldlist = world;
}

entitytype::entitytype() {
}

entitytype::~entitytype() {

}

action* entitytype::getAction( std::string name) {
    for( int i = 0; i < (int)p_actions.size(); i++) {
        if( p_actions[i].name == name)
            return &p_actions[i];
    }

    return NULL;
}

void entitytype::addAction( std::string name, std::string file, int frame, int speed, int loop, image *image) {
    action *l_action = new action;

    l_action->name = name;
    l_action->file = file;
    l_action->frame = frame;
    l_action->speed = speed;
    l_action->imagefile = image;
    l_action->loop = loop;

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

entity::entity( int id)
{
    // set id
    p_id = id;
    p_direction = 0;

    p_state = NULL;

    // timer start
    p_timer.start();

    p_timestartaction = 0;

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

    if( l_action->loop == 0) {
        if( l_action->frame-1 != (int)(p_actionframe/l_action->speed)%l_action->frame)
            p_actionframe = graphic->getFrame() - p_timestartaction;
    }  else {
        p_actionframe = graphic->getFrame() - p_timestartaction;
    }

    if( l_action->frame != 0)
        l_frame = p_type->getWidth()*( ((int)(p_actionframe/l_action->speed) ) %l_action->frame);
    else
        l_frame = 0;
    graphic->drawImage( l_image, p_pos.tovec2(), vec2( p_type->getWidth(),p_type->getHeight()), vec2( l_frame, 0), 0, p_direction);
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


entitylist::entitylist()
{
    // start at 0
    p_id = 1;
    p_playerentity = 0;
}

entitylist::~entitylist()
{
    //dtor
}

int entitylist::create( entitytype *type, vec2 pos) {
    entity* obj;

    // check data
    if( type == NULL)
        return -1;

    printf( "creating \"%s\" with id %d\n", type->getName().c_str(), p_id);

    // create object
    obj = new entity( p_id);
    // set data
    obj->setAction( ACTION_IDLE);
    obj->setType( type);
    obj->setPos( pos);
    obj->setVertex( type->getVertex());
    obj->setSolid( type->getIsSolid());

    // player entity incress if he is one
    if( type->getIsPlayer())
        p_playerentity++;

    obj->loadScript( type->getScriptName());

    // add to vector
    p_entitys.push_back( *obj);

    // start script if write
    getEntity( p_id)->lua_start( p_id);

    // incress next id
    p_id++;

    return (p_id-1);
}

void entitylist::deleteObj( int id) {
    for( int i = 0; i < (int)p_entitys.size(); i++)
        if( p_entitys[i].getId() == id)
            p_entitys.erase( p_entitys.begin()+i);
}

bool entitylist::createFromWorldFile( std::string file, world *world) {
    XMLDocument l_file;
    std::string l_type;

    vec2 l_pos;

    // load form world file
    XMLError l_result = l_file.LoadFile( file.c_str());
    // check the file
    XMLCheckResult(l_result);

    // file exist?
    if( file_exist( file) == false) {
        printf( "entitylist::createFromWorldFile file dont exist \"%s\"\n", file.c_str());
        return false;
    }

    XMLElement* l_map = l_file.FirstChildElement( "map" );
    if( !l_map) {
        printf( "entitylist::createFromWorldFile world has no map defined\n");
        return false;
    }

    XMLElement* l_objectgroup = l_map->FirstChildElement( "objectgroup" );
    if( !l_objectgroup) {
        printf( "entitylist::createFromWorldFile objectgroup has no map defined\n");
        return false;
    }

    XMLElement* l_object = l_objectgroup->FirstChildElement( "object" );

    int l_id = 0;
    //
    while( l_object) {

        // load data
        l_type = l_object->Attribute("name") == NULL?"":l_object->Attribute("name");
        l_pos.x = atoi(l_object->Attribute( "x"));
        l_pos.y = atoi(l_object->Attribute( "y"));


        //printf("%s %d %d\n", l_type.c_str(), l_pos.x, l_pos.y);

        //
        entitytype *l_entity_type = getType( l_type);

        if( l_entity_type != NULL) {
            l_id = create( l_entity_type, l_pos);
            entity *l_entity = getEntity( l_id);

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
                else if( l_property == "global") {
                    // gloabl value
                    lua_pushstring( l_entity->lua_getLua(), l_value.c_str());
                    lua_setglobal( l_entity->lua_getLua(), "global_value");
                }

                l_xml_property = l_xml_property->NextSiblingElement( "property");
            }
        } else if( l_type == "start"){ // wolrd start point
            world->setStartPoint( l_pos);
        }

        // next object
        l_object = l_object->NextSiblingElement( "object");
    }

    return true;
}

void entitylist::draw(graphic *graphic, config *config) {
    for(int i = 0; i < (int)p_entitys.size(); i++) {
        entity *l_obj = &p_entitys[i];
        if( l_obj != NULL)
            l_obj->draw( graphic);

            // debug
            if( config->getDebug() ) {
                SDL_Rect rect = {   l_obj->getPosition().x + l_obj->getType()->getHitboxOffset().x,
                                    l_obj->getPosition().y + l_obj->getType()->getHitboxOffset().y,
                                    l_obj->getType()->getHitbox().x,
                                    l_obj->getType()->getHitbox().y};
                rect.x -= graphic->getCamera().x;
                rect.y -= graphic->getCamera().y;

                SDL_SetRenderDrawColor( graphic->getRenderer(), 255, 0, 0, 255);
                SDL_RenderDrawRect( graphic->getRenderer(), &rect );

                for( int n = 0; n < (int)l_obj->getVertex()->size(); n++) {
                    int l_blue = 0;
                    vertex *l_vertex = &l_obj->getVertex()->at(n);
                    fvec2 l_position = l_obj->getPosition();
                    vec2 l_collision_pos = l_position.tovec2() + l_vertex->pos - graphic->getCamera().tovec2();

                    if( l_vertex->hit == true)
                        l_blue = 255;
                    SDL_SetRenderDrawColor( graphic->getRenderer(), l_blue, 255, l_blue, 255);

                    SDL_RenderDrawPoint( graphic->getRenderer(), l_collision_pos.x, l_collision_pos.y);
                }
            }

    }
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
    float l_collision_y, l_collision_x;
    int l_vertexhitchange = 0;
    vec2 l_iposition;

    // calc delta of velocity
    l_velocityDelta = (float)deltaTime * world->getGravity();

    // again and again if we found one more to delete we search again
    for(int i = 0; i < (int)p_entitys.size(); i++) {
        if( p_entitys[i].isbedelete == true ) {
            p_entitys.erase( p_entitys.begin() + i);
            i--;
        }
    }

    // go through
    for(int i = 0; i < (int)p_entitys.size(); i++) {
        entity *l_entity = &p_entitys[i];

        if( l_entity == NULL)
            continue;

        // no calc anymore if he flag too destory
        if( l_entity->isbedelete == true )
            continue;

        entitytype *l_type = l_entity->getType();

        l_vertexhitchange = 0;

        // lua timer for simple ai
        if( l_type->getTimerTime() > 0) {
            // time over
            if( l_entity->getTimer()->getTicks() > l_type->getTimerTime() ) {
                // start again
                l_entity->getTimer()->start();
                l_entity->lua_timer( l_entity->getId(), l_type->getTimerTime());
            }
        }

        // liquid
        if( l_type->getGravity() == true && l_type->getHitboxOffset().x != 0) {
            // umrechnen
            l_iposition = (l_entity->getPosition().tovec2()+l_type->getHitboxOffset()+l_type->getHitbox()/vec2( 2, 2 ) + world->getTileSize()/vec2( 2, 2 )-(world->getTileSize()/vec2( 2, 2) ) )/world->getTileSize();
            tile *l_tile = world->getTile( world->getCollsionMap(), l_iposition);

            if( l_tile != NULL && l_tile->type != NULL) {
                // only once
                if(l_entity->isInLiquid() == false)
                    l_entity->setLiquid( true); // he swim
            } else if(l_entity->isInLiquid() == true)
                l_entity->setLiquid( false); // he now not swim anymore
        }

        // check abount hitbox
        std::vector<int> l_ids = collision_boundingBox( l_entity);
        if( l_ids.size() > 0)
            l_entity->lua_collision( l_entity->getId(), l_ids);

        fvec2 l_velocity;
        fvec2 l_position;
        fvec2 l_change;

        // positon ermiteln
        l_position = l_entity->getPosition();
        l_velocity = l_entity->getVelocity();

        // calc gravity
        bool l_iscalc_y = false;

        if( l_type->getGravity() == true ) {
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

        l_entity->setColisionDown( false);
        l_entity->setColisionUp( false);
        l_entity->setColisionLeft( false);
        l_entity->setColisionRight( false);


        fvec2 l_result_change = l_change;
        fvec2 l_result_velocity = l_velocity;

        for( int n = 0; n < (int)l_entity->getVertex()->size(); n++) {
            vertex *l_vertex = &l_entity->getVertex()->at(n);
            fvec2 l_collision_pos = l_position + l_vertex->pos;

            l_vertex->hit = false;

            bool l_changed = false;

            // collison
            if( l_vertex->down && l_velocity.y+0.001f > 0 ) {
                tile *l_tile = NULL;
                l_tile = world->getCollisionTileY( l_collision_pos, l_change, l_velocity);

                if( l_tile != NULL ) {
                    // zwischen rechnung
                    float l_pos_change_y = l_collision_pos.y + l_change.y;
                    float l_bottom = (l_tile->pos.y)*world->getTileSize().y;

                    // ausrechnung der änderung
                    float l_result = l_pos_change_y-l_bottom;

                    if(fabs(l_result) > l_velocity.y) {

                        if( l_changed == false ) {
                            l_changed = true; // we changed
                            // result
                            l_result_change = l_change - fvec2( 0, l_result);

                            //l_change.y = 0;//l_entity->( fvec2());
                            l_result_velocity.y = 0;
                        }


                        l_vertexhitchange += setVertexHit( l_vertex, true);
                        l_entity->setColisionDown( true);
                    }
                }

                // object collision
                if( l_ids.size() > 0 && l_change.y > 0) {
                    for( int i = 0; i < (int)l_ids.size(); i++) {
                        entity *l_obj = getEntity( l_ids[i]);
                        // if is solid
                        if( l_obj->isSolid()) {
                          // zwischen rechnung
                            float l_pos_change_y = l_collision_pos.y + l_change.y;
                            float l_bottom = l_obj->getPosition().y + l_obj->getType()->getHitboxOffset().y;

                            float l_result = l_pos_change_y-l_bottom;

                            if( fabs(l_result) > l_velocity.y && l_result < l_change.y+5.f) {
                                if( l_changed == false ) {
                                    l_changed = true; // we changed
                                    l_result_change = l_change - fvec2( 0, l_result);

                                    //l_change.y = 0;//l_entity->( fvec2());
                                    l_result_velocity.y = l_obj->getVelocity().y>0.0f?l_obj->getVelocity().y:0;

                                    // x achse
                                    l_result_change.x += l_obj->getVelocity().x*(deltaTime);
                                }

                                l_iscalc_y = true;

                                l_vertexhitchange += setVertexHit( l_vertex, true);
                                l_entity->setColisionDown( true);
                            }
                        }
                    }
                }
            }



            // collison
            if( l_vertex->up && l_velocity.y < 0) {
                tile *l_tile = NULL;
                l_tile = world->getCollisionTileY( l_collision_pos, l_change, l_velocity);

                if( l_tile ) {
                    // zwischen rechnung
                    float l_pos_change_y = l_collision_pos.y + l_change.y;
                    l_pos_change_y -= world->getTileSize().y;
                    float l_bottom = (l_tile->pos.y)*world->getTileSize().y;

                    // ausrechnung der änderung
                    float l_result = l_pos_change_y-l_bottom;

                    if( fabs(l_result) > l_velocity.y ) {
                        if( l_changed == false ) {
                            l_changed = true; // we changed

                            l_result_change = l_change - fvec2( 0, l_result);

                            //l_change.y = 0;//l_entity->( fvec2());
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

                            if(fabs(l_result) > l_velocity.y && fabs(l_result) < world->getTileSize().y) {
                                if( l_changed == false ) {
                                    l_changed = true; // we changed
                                    l_change = l_change - fvec2( 0, l_result);

                                    //l_change.y = 0;//l_entity->( fvec2());
                                    l_velocity.y = l_obj->getVelocity().y>0.0f?l_obj->getVelocity().y:0;
                                }

                                l_iscalc_y = true;

                                l_vertexhitchange += setVertexHit( l_vertex, true);
                                l_entity->setColisionUp( true);
                            }
                        }
                    }
                }
            }
        }

        // set new wert
        l_change = l_result_change;
        l_velocity = l_result_velocity;

        for( int n = 0; n < (int)l_entity->getVertex()->size(); n++) {
            vertex *l_vertex = &l_entity->getVertex()->at(n);
            fvec2 l_collision_pos = l_position + l_vertex->pos;

            bool l_changed = false;

            if( l_vertex->right && l_velocity.x + 0.001f > 0 && l_collision_pos.y > 0) {
                tile *l_tile = NULL;
                l_tile = world->getCollisionTileX( l_collision_pos, l_change, l_velocity);
                if( l_tile ) {
                    // zwischen rechnung
                    float l_pos_change_y = l_collision_pos.x + l_change.x;
                    float l_bottom = (l_tile->pos.x)*world->getTileSize().x;

                    // ausrechnung der änderung
                    float l_result = l_pos_change_y-l_bottom;

                    if(fabs(l_result) > 0.0 && l_result < l_change.x + 1.f ) {

                        if( l_changed == false ) {
                            l_changed = true;
                            l_result_change = l_change - fvec2( l_result, 0);

                            //l_change.y = 0;//l_entity->( fvec2());
                            l_result_velocity.x = 0;
                        }


                        l_vertexhitchange += setVertexHit( l_vertex, true);
                        l_entity->setColisionRight( true);
                    }
                }

                // map border
                if( l_collision_pos.x > world->getWorld().x*world->getTileSize().x) {
                    l_change = l_change - fvec2( l_change.x, 0);

                    //l_change.y = 0;//l_entity->( fvec2());
                    l_velocity.x = 0;

                    l_vertexhitchange += setVertexHit( l_vertex, true);
                    l_entity->setColisionRight( true);
                }

                // object collision
                if(  l_ids.size() > 0 && l_change.x > 0) {
                    for( int i = 0; i < (int)l_ids.size(); i++) {
                        entity *l_obj = getEntity( l_ids[i]);
                        if( l_obj->isSolid()) {

                          // zwischen rechnung
                            float l_pos_change_x = l_collision_pos.x + l_change.x+2.0f;
                            float l_bottom = l_obj->getPosition().x + l_obj->getType()->getHitboxOffset().x;

                            float l_result = l_pos_change_x-l_bottom;

                            if(fabs(l_result) > l_velocity.x && fabs(l_result) < world->getTileSize().x && l_iscalc_y == false) {
                                if( l_changed == false ) {
                                    l_changed = true;
                                    l_result_change = l_change - fvec2( l_result,0 );

                                    //l_change.y = 0;//l_entity->( fvec2());
                                    l_result_velocity.x = 0; //l_obj->getVelocity().y>0.0f?l_obj->getVelocity().y:0;
                                }


                                l_vertexhitchange += setVertexHit( l_vertex, true);
                                l_entity->setColisionRight( true);

                            }
                        }
                    }
                }
            }
            if( l_vertex->left && l_velocity.x - 0.001f < 0 && l_collision_pos.y > 0) {
                tile *l_tile = NULL;
                l_tile = world->getCollisionTileX( l_collision_pos, l_change, l_velocity);
                if( l_tile) {

                    // zwischen rechnung
                    float l_pos_change_y = l_collision_pos.x + l_change.x;
                    l_pos_change_y -= world->getTileSize().x;
                    float l_bottom = (l_tile->pos.x)*world->getTileSize().x;

                    // ausrechnung der änderung
                    float l_result = l_pos_change_y-l_bottom;

                    if( fabs(l_result) > 0.0 && l_tile->pos.y*world->getTileSize().y+world->getTileSize().y > l_collision_pos.y
                       && fabs( l_result) < fabs(l_change.x)+1.f ) {
                        if( l_changed == false ) {
                            l_changed = true;
                            l_result_change = l_change - fvec2( l_result, 0);

                            //l_change.y = 0;//l_entity->( fvec2());
                            l_result_velocity.x = 0;
                        }

                        l_vertexhitchange += setVertexHit( l_vertex, true);
                        l_entity->setColisionLeft( true);
                    }
                }

                if( l_collision_pos.x < 0) {
                    l_change = l_change - fvec2( l_change.x, 0);

                    //l_change.y = 0;//l_entity->( fvec2());
                    l_velocity.x = 0;

                    l_vertexhitchange += setVertexHit( l_vertex, true);
                    l_entity->setColisionLeft( true);
                }


                // object collision
                if( l_ids.size() > 0 && l_change.x < 0) {
                    for( int i = 0; i < (int)l_ids.size(); i++) {
                        entity *l_obj = getEntity( l_ids[i]);
                        if( l_obj->isSolid()) {

                          // zwischen rechnung
                            float l_pos_change_x = l_collision_pos.x + l_change.x - (l_velocity.x*deltaTime)-2.0f;
                            float l_bottom = l_obj->getPosition().x + l_obj->getType()->getHitboxOffset().x + l_obj->getType()->getHitbox().x;

                            float l_result = l_pos_change_x-l_bottom;

                            if(fabs(l_result) > l_velocity.x && fabs(l_result) < world->getTileSize().x && l_iscalc_y == false) {
                                if( l_changed == false ) {
                                    l_changed = true;
                                    l_result_change = l_change - fvec2( l_result,0 );

                                    //l_change.y = 0;//l_entity->( fvec2());
                                    l_result_velocity.x = 0; //l_obj->getVelocity().y>0.0f?l_obj->getVelocity().y:0;
                                }

                                l_vertexhitchange += setVertexHit( l_vertex, true);
                                l_entity->setColisionLeft( true);

                            }
                        }
                    }
                }
            }
        }

        // set new wert
        l_change = l_result_change;
        l_velocity = l_result_velocity;

        if( l_type->getGravity() == true) {
            if( l_entity->getColisionDown() && !l_entity->isInLiquid())
                l_velocity.x = l_velocity.x*0.8f;
            else if(l_entity->isInLiquid() == true)
                l_velocity.x = l_velocity.x*0.5f;
            else
                l_velocity.x = l_velocity.x*0.99f;
        }

        // set net position
        l_entity->setPos( l_position + l_change );

        // add velocity next frame
        l_entity->setVelocity( l_velocity);

        if( l_vertexhitchange > 0) {
            l_entity->lua_vertexhit( l_entity->getId());
        }
        /*if( p_entitys[i].getPosition().y+l_velocity.y > 250)
            p_entitys[i].setVelocity( fvec2( 0.1f, -0.375f) );*/
    }
}

std::vector <int> entitylist::collision_boundingBox( entity* checkentity) {
    entitytype *l_type = checkentity->getType();
    std::vector <int> l_id;

    vec2 l_rect1 = l_type->getHitboxOffset() + checkentity->getPosition().tovec2();
    vec2 l_rect1_size = l_type->getHitbox();
    //
    for( int i = 0; i < (int)p_entitys.size(); i++)  {
        entity* l_obj = &p_entitys[i];
        entitytype *l_typeobj = l_obj->getType();

        // self dont regist
        if( l_obj->getId() == checkentity->getId())
            continue;

        // calc rect 2
        vec2 l_rect2 = l_typeobj->getHitboxOffset() + l_obj->getPosition().tovec2();
        vec2 l_rect2_size = l_typeobj->getHitbox();

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

    p_folder = folder;

    std::string l_pathfile = folder + (char*)ENTITY_FILE;

    int l_width = 0;
    int l_height = 0;
    bool l_gravity = true;
    bool l_isplayer = false;
    int l_timer = 0;
    bool l_solid = 0;
    bool l_isEnemy = 0;
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

    entitytype *l_type = new entitytype();
    std::string l_action_name;
    std::string l_action_file;
    int l_action_frame;
    int l_action_speed;
    int l_action_loop;

    bool l_idle = false;
    // read all actions
    XMLElement* l_xml_action = l_object->FirstChildElement( "action" );
    while( l_xml_action) {
        l_action_file = l_xml_action->GetText();
        l_action_name = l_xml_action->Attribute( "name" );
        if( l_xml_action->Attribute( "frame" ))
            l_action_frame = atoi( l_xml_action->Attribute( "frame" ));
        else
            l_action_frame = 0;
        if( l_xml_action->Attribute( "speed" ))
            l_action_speed = atoi( l_xml_action->Attribute( "speed" ));
        else
            l_action_speed = 0;

        if( l_xml_action->Attribute( "loop" ))
            l_action_loop = atoi( l_xml_action->Attribute( "loop" ));
        else
            l_action_loop = 1;

        // check data
        if( l_action_speed == 0)
            l_action_speed = 1; // verry fast animation

        if( l_action_name == ACTION_IDLE)
            l_idle = true;

        // push back
        l_type->addAction( l_action_name, l_action_file, l_action_frame, l_action_speed, l_action_loop,graphic->loadImage( folder + l_action_file));

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


    if( !l_idle) {
        printf("entitylist::loadType type %s has no idle action!\n", l_name.c_str());
        return false;
    }

    l_type->setWidth(l_width);
    l_type->setHeight(l_height);
    l_type->setName( l_name);
    l_type->setGravity( l_gravity);
    l_type->setIsPlayer( l_isplayer);
    l_type->setScriptName( l_script);
    l_type->setSolid( l_solid);
    l_type->setTimer( l_timer);
    l_type->setIsEnemy( l_isEnemy);

    p_entity_types.push_back( *l_type);

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
        if( p_entity_types[i].getName() == name)
            return &p_entity_types[i];
    return NULL;
}

std::vector<int> entitylist::findPlayerObject() {
    std::vector<int> l_obj;

    for( int i = 0; i < (int)p_entitys.size(); i++) {
        entity *l_entity = &p_entitys[i];
        if( l_entity->getType()->getIsPlayer() && l_entity->getAction() != "die")
            l_obj.push_back( l_entity->getId());
    }
    return l_obj;
}

entity* entitylist::getEntity( int id) {
    for( int i = 0; i < (int)p_entitys.size(); i++)
        if( p_entitys[i].getId() == id)
            return &p_entitys[i];
    return NULL;
}

