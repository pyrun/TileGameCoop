#include "entity.h"

using namespace tinyxml2;

#ifndef XMLCheckResult
	#define XMLCheckResult(a_eResult) if (a_eResult != XML_SUCCESS) { printf("Error: %i\n", a_eResult); return a_eResult; }
#endif

/** LUA FUNCTION */

entitylist *lua_entitylist = NULL;

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

    return 0;
}

static int lua_setVelocityX( lua_State *state) {
    entity *l_obj;
    int l_id;
    float l_x;
    if ( !lua_isnumber( state, 1) || !lua_isnumber( state, 2)) {
        printf( "lua_addVelocity call wrong argument\n");
        return 0;
    }

    l_id = lua_tointeger( state, 1);
    l_x = lua_tonumber( state, 2);

    l_obj = lua_entitylist->getEntity( l_id);
    if( l_obj == NULL) {
        printf( "lua_addVelocity obj not found\n");
        return 0;
    }
    l_obj->setVelocity( fvec2( l_x, l_obj->getVelocity().y) );

    return 0;
}

static int lua_getVelocity( lua_State *state) {
    entity *l_obj;
    int l_id;
    if ( !lua_isnumber( state, 1) ) {
        printf( "lua_addVelocity call wrong argument\n");
        return 0;
    }

    l_id = lua_tointeger( state, 1);

    l_obj = lua_entitylist->getEntity( l_id);
    if( l_obj == NULL) {
        printf( "lua_addVelocity obj not found\n");
        return 0;
    }
    lua_pushnumber( state, l_obj->getVelocity().x );
    lua_pushnumber( state, l_obj->getVelocity().y );

    return 2;
}

static int lua_setVelocityY( lua_State *state) {
    entity *l_obj;
    int l_id;
    float l_y;
    if ( !lua_isnumber( state, 1) || !lua_isnumber( state, 2)) {
        printf( "lua_addVelocity call wrong argument\n");
        return 0;
    }

    l_id = lua_tointeger( state, 1);
    l_y = lua_tonumber( state, 2);

    l_obj = lua_entitylist->getEntity( l_id);
    if( l_obj == NULL) {
        printf( "lua_addVelocity obj not found\n");
        return 0;
    }
    l_obj->setVelocity( fvec2( l_obj->getVelocity().x, l_y) );

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


void lua_install( lua_State *state) {
    // add all entity function ..
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
}

void lua_setEntity( entitylist *entity) {
    // set list
    lua_entitylist = entity;
}

entitytype::entitytype() {
    p_state = NULL;
}

entitytype::~entitytype() {
    if( p_state)
        lua_close( p_state);
}

void entitytype::addAction( std::string name, std::string file, int frame, int speed, image *image) {
    action *l_action = new action;

    l_action->name = name;
    l_action->file = file;
    l_action->frame = frame;
    l_action->speed = speed;
    l_action->imagefile = image;

    p_actions.push_back( *l_action);
}

void entitytype::addVertex(vec2 pos, bool left, bool right, bool up, bool down) {
    vertex *l_vertex = new vertex;

    l_vertex->pos = pos;
    l_vertex->left = left;
    l_vertex->right = right;
    l_vertex->down = down;
    l_vertex->up = up;

    p_vertex.push_back( *l_vertex);
}

void entitytype::loadScript( std::string file) {
    int l_result;

    if( file.size() < 1) // no file
        return;

    if( file_exist( file) == false) {
        printf( "entitytype::loadScript file '%s' dont found\n", file.c_str());
        return;
    }

    p_state = luaL_newstate();

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

void entitytype::lua_jump( int id) {
    if( p_state == NULL)
        return;
    // name the function
    lua_getglobal( p_state, "jump");
    if( !lua_isfunction( p_state, -1)) {
        lua_pop( p_state,1);
        return;
    }
    lua_pushnumber( p_state, id);
    // call the function
    if( lua_pcall( p_state, 1, 0, 0))
        printf("entitytype::lua_jump %s\n", lua_tostring( p_state, -1));
}

void entitytype::lua_right( int id) {
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
        printf("entitytype::lua_right %s\n", lua_tostring( p_state, -1));
}

void entitytype::lua_left( int id) {
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
        printf("entitytype::lua_left %s\n", lua_tostring( p_state, -1));
}
void entitytype::lua_up( int id) {
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
        printf("entitytype::lua_up %s\n", lua_tostring( p_state, -1));
}
void entitytype::lua_down( int id) {
    if( p_state != NULL)
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
        printf("entitytype::lua_down %s\n", lua_tostring( p_state, -1));
}

void entitytype::lua_printerror() {
  // The error message is on top of the stack.
  // Fetch it, print it and then pop it off the stack.
  const char* message = lua_tostring( p_state, -1);
  puts(message);
  lua_pop( p_state, 1);
}

action* entitytype::getAction( std::string name) {
    for( int i = 0; i < (int)p_actions.size(); i++) {
        if( p_actions[i].name == name)
            return &p_actions[i];
    }

    return NULL;
}

entity::entity( int id)
{
    // set id
    p_id = id;
}

entity::~entity()
{
    //dtor
}

void entity::draw( graphic *graphic) {
    int l_frame;

    action *l_action = p_type->getAction( this->p_action);
    if( l_action == NULL) { // falls animaton fehlt zurück zu idle
        p_action = ACTION_IDLE;
        printf("dont found action, jump to idle\n");
        return;
    }

    image *l_image = l_action->imagefile;


    if( l_action->frame != 0)
        l_frame = p_type->getWidth()*( ((int)(graphic->getFrame()/l_action->speed) ) %l_action->frame);
    else
        l_frame = 0;
    graphic->drawImage( l_image, p_pos.tovec2(), vec2( p_type->getWidth(),p_type->getHeight()), vec2( l_frame, 0));
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

    // incress next id
    p_id++;

    // player entity incress if he is one
    if( type->getIsPlayer())
        p_playerentity++;

    // add to vector
    p_entitys.push_back( *obj);

    return (p_id-1);
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

    //

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

void entitylist::process( world *world, int deltaTime) {
    float l_velocityDelta;
    float l_collision_y, l_collision_x;

    // calc delta of velocity
    l_velocityDelta = (float)deltaTime * world->getGravity();

    for(int i = 0; i < (int)p_entitys.size(); i++) {
        entity *l_entity = &p_entitys[i];
        entitytype *l_type = l_entity->getType();


        /*if( l_type->lua_hasLoaded())
            l_type->lua_right();*/

        fvec2 l_velocity;
        fvec2 l_position;
        fvec2 l_change;

        // calc gravity
        if( l_type->getGravity() == false) {
            // positon ermiteln
            l_position = l_entity->getPosition();
            l_velocity = l_entity->getVelocity();

            // änderung rechnen
            l_change.x += l_velocity.x * deltaTime;
            if( l_velocityDelta)
                l_change.y += (( l_velocity.y + (l_velocityDelta / 2)) * deltaTime);

            // y delta dazurechnen (x nicht nötig da keine gravi. )
            l_velocity.y += l_velocityDelta;
            l_collision_y = 0.0f;
            l_collision_x = 0.0f;

            l_entity->setColisionDown( false);
            l_entity->setColisionUp( false);
            l_entity->setColisionLeft( false);
            l_entity->setColisionRight( false);


            for( int n = 0; n < (int)l_type->getVertex().size(); n++) {
                vertex *l_vertex = &l_type->getVertex()[n];
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
                }
            }
            if( l_collision_y != MASSIV_TILE) {
                l_change = l_change - fvec2( 0, l_collision_y);
                //l_entity->setPos( l_position + l_change - fvec2( 0, l_collision_y) );
                l_entity->setVelocity( fvec2() );
                l_velocity.y = 0;
                //continue;
            }
            if( l_collision_x != MASSIV_TILE) {
                l_change = l_change - fvec2( l_collision_x, 0);
                //l_entity->setPos( l_position + l_change - fvec2( 0, l_collision_y) );
                l_entity->setVelocity( fvec2() );
                l_velocity.x = 0;
                //continue;
            }
            if( l_entity->getColisionDown())
                l_velocity.x = l_velocity.x/3;
            else
                l_velocity.x = l_velocity.x*0.99;
            l_entity->setPos( l_position + l_change );

            // add velocity next frame
            l_entity->setVelocity( l_velocity);
        }

        /*if( p_entitys[i].getPosition().y+l_velocity.y > 250)
            p_entitys[i].setVelocity( fvec2( 0.1f, -0.375f) );*/
    }
}

bool entitylist::loadType( std::string folder, graphic *graphic) {
    XMLDocument l_file;

    p_folder = folder;

    std::string l_pathfile = folder + (char*)ENTITY_FILE;

    int l_width;
    int l_height;
    bool l_gravity;
    bool l_isplayer;
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
    if( !l_object)
        return false;

    l_name = l_object->Attribute( "name" );
    l_width = atoi(l_object->Attribute( "width" ));
    l_height = atoi(l_object->Attribute( "height" ));
    l_gravity = atoi(l_object->Attribute( "gravity" ));
    l_isplayer = atoi(l_object->Attribute( "player" ));

    if( l_object->Attribute( "script"))
        l_script = folder + l_object->Attribute( "script");

    entitytype *l_type = new entitytype();
    std::string l_action_name;
    std::string l_action_file;
    int l_action_frame;
    int l_action_speed;

    bool l_idle = false;
    // read all actions
    XMLElement* l_xml_action = l_object->FirstChildElement( "action" );
    while( l_xml_action) {
        l_action_file = l_xml_action->GetText();
        l_action_name = l_xml_action->Attribute( "name" );
        l_action_frame = atoi( l_xml_action->Attribute( "frame" ));
        l_action_speed = atoi( l_xml_action->Attribute( "speed" ));


        // check data
        if( l_action_speed == 0)
            l_action_speed = 1; // verry fast animation

        if( l_action_name == ACTION_IDLE)
            l_idle = true;

        // push back
        l_type->addAction( l_action_name, l_action_file, l_action_frame, l_action_speed, graphic->loadImage( folder + l_action_file));

        l_xml_action = l_xml_action->NextSiblingElement("action");
    }

    // load vertex
    std::string l_vertex_name;
    vec2 l_vertex_pos;
    bool l_vertex_up;
    bool l_vertex_left;
    bool l_vertex_down;
    bool l_vertex_right;
    XMLElement* l_xml_vertex = l_object->FirstChildElement( "vertex" );
    while( l_xml_vertex) {
        l_vertex_name = l_xml_vertex->GetText();
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
        l_type->addVertex( l_vertex_pos, l_vertex_left, l_vertex_right, l_vertex_up, l_vertex_down);

        // next vertex
        l_xml_vertex = l_xml_vertex->NextSiblingElement("vertex");
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
    l_type->loadScript( l_script);
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
        if( l_entity->getType()->getIsPlayer())
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

