#include "entity.h"

using namespace tinyxml2;

#ifndef XMLCheckResult
	#define XMLCheckResult(a_eResult) if (a_eResult != XML_SUCCESS) { printf("Error: %i\n", a_eResult); return a_eResult; }
#endif

/** LUA FUNCTION */

entitylist *lua_entitylist = NULL;

static int lua_isenemy( lua_State *state) {
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

    lua_entitylist->deleteObj( l_id);
    return 0;
}


static int lua_createObject( lua_State *state) {
    entity *l_obj;
    int l_id;
    bool l_alive;
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
    if( l_alive = true && l_obj != NULL)
        if( l_obj->getAction() == "die")
            l_alive = false;

    lua_pushboolean( state, l_alive);

    return 1;
}

static int lua_setAnimation( lua_State *state) {
    entity *l_obj;
    int l_id;
    std::string l_name;

    if( !lua_isnumber( state, 1) || !lua_isstring( state, 2) ) {
        printf( "lua_setAnimation call wrong argument\n");
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
    int l_dir;

    if( !lua_isnumber( state, 1) || !lua_isnumber( state, 2) ) {
        printf( "lua_setAnimation call wrong argument\n");
        return 0;
    }

    l_id = lua_tointeger( state, 1);
    l_dir = lua_tointeger( state, 2);

    l_obj = lua_entitylist->getEntity( l_id);
    if( l_obj == NULL) {
        printf( "lua_addVelocity obj not found\n");
        return 0;
    }

    l_obj->setDirection( l_dir);
    return 0;
}

static int lua_getAnimationDirection( lua_State *state) {
    entity *l_obj;
    int l_id;
    int l_dir;

    if( !lua_isnumber( state, 1)) {
        printf( "lua_setAnimation call wrong argument\n");
        return 0;
    }

    l_id = lua_tointeger( state, 1);

    l_obj = lua_entitylist->getEntity( l_id);
    if( l_obj == NULL) {
        printf( "lua_addVelocity obj not found\n");
        return 0;
    }

    l_dir = l_obj->getDirection();
    lua_pushnumber( state, l_dir );
    return 1;
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
    lua_pushcfunction( state, lua_isenemy);
    lua_setglobal( state, "isenemy");

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

    lua_pushcfunction( state, lua_getAnimationDirection);
    lua_setglobal( state, "getAnimationDirection");

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

void lua_setEntity( entitylist *entity) {
    // set list
    lua_entitylist = entity;
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

    // start script if write
    lua_start( id);
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
    if( p_state == NULL)
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
    p_id = 0;
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

    // incress next id
    p_id++;

    // player entity incress if he is one
    if( type->getIsPlayer())
        p_playerentity++;

    obj->loadScript( type->getScriptName());

    // add to vector
    p_entitys.push_back( *obj);

    return (p_id-1);
}

void entitylist::deleteObj( int id) {
    for( int i = 0; i < (int)p_entitys.size(); i++)
        if( p_entitys[i].getId() == id)
            p_entitys.erase( p_entitys.begin()+i);
}

void entitylist::createFromWorldFile( std::string file) {
    XMLDocument l_file;
    std::string l_type;

    vec2 l_pos;

    // load form world file
    XMLError l_result = l_file.LoadFile( file.c_str());

    // file exist?
    if( file_exist( file) == false) {
        printf( "entitylist::createFromWorldFile file dont exist \"%s\"\n", file.c_str());
        return;
    }

    XMLElement* l_map = l_file.FirstChildElement( "map" );
    if( !l_map) {
        printf( "entitylist::createFromWorldFile world has no map defined\n");
        return;
    }

    XMLElement* l_objectgroup = l_map->FirstChildElement( "objectgroup" );
    if( !l_objectgroup) {
        printf( "entitylist::createFromWorldFile objectgroup has no map defined\n");
        return;
    }

    XMLElement* l_object = l_objectgroup->FirstChildElement( "object" );

    //
    while( l_object) {

        // load data
        l_type = l_object->Attribute("name") == NULL?"":l_object->Attribute("name");
        l_pos.x = atoi(l_object->Attribute( "x"));
        l_pos.y = atoi(l_object->Attribute( "y"));


        //printf("%s %d %d\n", l_type.c_str(), l_pos.x, l_pos.y);

        //
        entitytype *l_entity_type = getType( l_type);

        if( l_entity_type != NULL)
            create( l_entity_type, l_pos);

        // next object
        l_object = l_object->NextSiblingElement( "object");
    }
}

void entitylist::draw(graphic *graphic) {
    for(int i = 0; i < (int)p_entitys.size(); i++)
        p_entitys[i].draw( graphic);
}

int entitylist::setVertexHit( vertex *vertex, bool set){
    bool l_old;

    l_old = vertex->hit;
    vertex->setHit( set);

    if( l_old != set)
        return 1;
    return 0;
}

void entitylist::process( world *world, int deltaTime) {
    float l_velocityDelta;
    float l_collision_y, l_collision_x;
    int l_vertexhitchange = 0;
    vec2 l_iposition;

    // calc delta of velocity
    l_velocityDelta = (float)deltaTime * world->getGravity();

    // again and angain if wie found one more to delete
    for(int i = 0; i < (int)p_entitys.size(); i++) {
        if( p_entitys[i].isbedelete == true ) {
            p_entitys.erase( p_entitys.begin() + i);
            i--;
        }
    }

    // go through
    for(int i = 0; i < (int)p_entitys.size(); i++) {
        entity *l_entity = &p_entitys[i];

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
            l_iposition = (l_entity->getPosition().tovec2()+l_type->getHitboxOffset()+l_type->getHitbox()/vec2( 2, 2 ) + world->getTileSize()/vec2( 2, 2 ) )
            /world->getTileSize();
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

        // calc gravity
        if( l_type->getGravity() == true ) {
            // positon ermiteln
            l_position = l_entity->getPosition();
            l_velocity = l_entity->getVelocity();

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

            l_collision_y = 0.0f;
            l_collision_x = 0.0f;

            l_entity->setColisionDown( false);
            l_entity->setColisionUp( false);
            l_entity->setColisionLeft( false);
            l_entity->setColisionRight( false);

            for( int n = 0; n < (int)l_entity->getVertex()->size(); n++) {
                vertex *l_vertex = &l_entity->getVertex()->at(n);
                vec2 l_collision_pos = l_vertex->pos;

                // collison
                if( l_vertex->down) {
                    float l_temp;

                    // collsion y
                    l_temp = world->getCollisionY( l_position + l_collision_pos, l_change, l_velocity);

                    // nur falls alter wert kleiner ist
                    if( ((l_collision_y == MASSIV_TILE || l_collision_y == 0.0f) && l_temp != MASSIV_TILE) ||
                       (l_collision_y < l_temp && l_temp != MASSIV_TILE) ||
                       (l_collision_y == 0.0f)) {
                        l_collision_y = l_temp;
                    }

                    if( l_collision_y != MASSIV_TILE)
                        l_entity->setColisionDown( true);

                    // lua event
                    if( l_temp != MASSIV_TILE)
                        l_vertexhitchange += setVertexHit( l_vertex, true);
                    else l_vertexhitchange += setVertexHit( l_vertex, false);
                }
                if( l_vertex->up) {
                    float l_temp;

                    // collsion y
                    l_temp = world->getCollisionY( l_position + l_collision_pos, l_change, l_velocity, true);

                    // nur falls alter wert kleiner ist
                    if( ((l_collision_y == MASSIV_TILE || l_collision_y == 0.0f) && l_temp != MASSIV_TILE) ||
                       (l_collision_y > l_temp && l_temp != MASSIV_TILE) ||
                       (l_collision_y == 0.0f)) {
                        l_collision_y = l_temp;
                        //printf("%.2f\n", l_temp);
                    }

                    if( l_collision_y != MASSIV_TILE)
                        l_entity->setColisionUp( true);

                    // lua event
                    if( l_temp != MASSIV_TILE)
                        l_vertexhitchange += setVertexHit( l_vertex, true);
                    else l_vertexhitchange += setVertexHit( l_vertex, false);
                }
                if( l_vertex->right) {
                    float l_temp;

                    // collsion y
                    l_temp = world->getCollisionX( l_position + l_collision_pos, l_change, l_velocity);

                    // nur falls alter wert kleiner ist
                    if( ((l_collision_x == MASSIV_TILE || l_collision_x == 0.0f) && l_temp != MASSIV_TILE) ||
                       (l_collision_x > l_temp && l_temp != MASSIV_TILE) ||
                       (l_collision_x == 0.0f)) {
                        l_collision_x = l_temp;
                    }
                    if( l_collision_x != MASSIV_TILE)
                        l_entity->setColisionRight( true);

                    // lua event
                    if( l_temp != MASSIV_TILE)
                        l_vertexhitchange += setVertexHit( l_vertex, true);
                    else l_vertexhitchange += setVertexHit( l_vertex, false);
                }


                if( l_vertex->left) {
                    float l_temp;

                    // collsion y
                    l_temp = world->getCollisionX( l_position + l_collision_pos, l_change, l_velocity, true);

                    // nur falls alter wert kleiner ist
                    if( ((l_collision_x == MASSIV_TILE || l_collision_x == 0.0f) && l_temp != MASSIV_TILE) ||
                       (l_collision_x < l_temp && l_temp != MASSIV_TILE) ||
                       (l_collision_x == 0.0f)) {
                        l_collision_x = l_temp;
                    }
                    if( l_collision_x != MASSIV_TILE)
                        l_entity->setColisionLeft( true);

                    // lua event
                    if( l_temp != MASSIV_TILE)
                        l_vertexhitchange += setVertexHit( l_vertex, true);
                    else l_vertexhitchange += setVertexHit( l_vertex, false);
                }
            }
            if( l_collision_y != MASSIV_TILE) {
                l_change = l_change - fvec2( 0, l_collision_y);
                //l_entity->setPos( l_position + l_change - fvec2( 0, l_collision_y) );
                l_entity->setVelocity( fvec2() );
                l_entity->setUpdate( true);
                l_velocity.y = 0;
            }
            if( l_collision_x != MASSIV_TILE) {
                l_change = l_change - fvec2( l_collision_x, 0);
                //l_entity->setPos( l_position + l_change - fvec2( 0, l_collision_y) );
                l_entity->setVelocity( fvec2() );
                l_entity->setUpdate( true);
                l_velocity.x = 0;
            }

            if( l_entity->getColisionDown() && !l_entity->isInLiquid())
                l_velocity.x = l_velocity.x*0.9f;
            else if(l_entity->isInLiquid() == true)
                l_velocity.x = l_velocity.x*0.5f;
            else
                l_velocity.x = l_velocity.x*0.99f;
            l_entity->setPos( l_position + l_change );

            // add velocity next frame
            l_entity->setVelocity( l_velocity);
        }

        if( l_vertexhitchange > 0)
            l_entity->lua_vertexhit( l_entity->getId());
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
    std::string l_script;

    std::string l_name;
    std::vector<action> *l_actions = new std::vector<action>();

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
            l_hitbox_offset.x = atoi( l_xml_hitbox->Attribute( "offsety"));
        else
            l_hitbox_offset.x = 0;

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
    l_type->setTimer( l_timer);
    lua_setEntity( this);

    p_entity_types.push_back( *l_type);

    return true;
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

