#ifndef ENTITY_H
#define ENTITY_H

#include <string>
#include <vector>

//#define old_physic

// debian
#ifdef IMADEBIAN
#include <lua5.3/lua.hpp>
#else
#include <lua.hpp>
#endif // IMADEBIAN

#include "../graphic/graphic.h"
#include "types.h"
#include "../xml/tinyxml2.h"
#include "world.h"
#include "particle.h"

#define ENTITY_FILE "definition.xml"

// grund action
#define ACTION_IDLE "idle"
#define ACTION_WALK "walk"
#define ACTION_RUN "run"
#define ACTION_JUMP "jump"
#define ACTION_ATTACK "attack"
#define ACTION_SWIM "swim"
#define ACTION_SPECIAL "special"
#define ACTION_DIE "die"

// collsion offset
#define COLLISION_X_OFFSET 6.0f
#define COLLISION_Y_OFFSET 6.0f

class action {
    public:
        std::string name;
        std::string file;
        int frames;
        int speed;
        int loop;
        image *imagefile;
        std::string startcall;
        std::string endcall;
};

class vertex {
    public:
        vec2 pos;
        bool up;
        bool left;
        bool right;
        bool down;
        bool hit;
        int id;

        void setHit( bool set) { hit = set; }
};

class entity_text {
    public:
        int id;
        std::string text;
};

class entitytype
{
    public:
        entitytype();
        virtual ~entitytype();

        action* getAction( std::string name);

        void addAction( std::string name, std::string file, int frames, int speed, int loop, image *image, std::string startcall, std::string endcall);
        void addVertex(vec2 pos, bool left, bool right, bool up, bool down, int id);

        void setName( std::string name) { p_name = name; }
        void setGravity( bool mass) { p_gravity = mass; }

        void setWidth( int width) { p_width = width; }
        void setHeight( int height) { p_height = height; }
        void setIsPlayer( bool isplayer) { p_isplayer = isplayer; }
        void setScriptName( std::string file) { p_script = file; }
        void setSolid( bool set) { p_solid = set; }
        void setTimer( int time) { p_time = time;}
        void setHitbox( vec2 offset, vec2 size) { p_hitbox_offset = offset; p_hitbox_size = size; }
        void setIsEnemy( bool set) { p_isEnemy = set; }

        std::string getName() { return p_name; }
        std::string getScriptName() { return p_script; }
        int getWidth() { return p_width; }
        int getHeight() { return p_height; }
        bool getGravity() { return p_gravity; }
        std::vector<vertex> getVertex() { return p_vertex; }
        bool getIsPlayer() { return p_isplayer; }
        int getTimerTime() { return p_time; }
        vec2 getHitbox() { return p_hitbox_size; }
        vec2 getHitboxOffset() { return p_hitbox_offset; }
        bool getIsSolid() { return p_solid; }
        bool getIsEnemy() { return p_isEnemy; }

    protected:

    private:
        int p_time;
        int p_width;
        int p_height;
        bool p_gravity;
        bool p_isplayer;
        std::string p_name;
        std::string p_script;
        std::vector<action> p_actions;
        std::vector<vertex> p_vertex;
        vec2 p_hitbox_offset;
        vec2 p_hitbox_size;
        bool p_solid;
        bool p_isEnemy;
};

class entity
{
    public:
        entity( int id);
        virtual ~entity();

        void draw( graphic *graphic);

        void setType( entitytype *type) { this->p_type = type; }
        void setAction( std::string name, bool withStartCall = true);
        void setTimeStartAction( int set) { p_timestartaction = set; }
        void setDirection( bool dir) { p_direction = dir; }
        int getDirection() { return p_direction; }
        void setPos( vec2 pos) { p_pos = pos; }
        void setPos( fvec2 pos) { p_pos = pos; }
        void setVelocity( fvec2 velocity) { p_velocity = velocity; }
        void setUpdate( bool set) { p_update = set; }
        void setVertex( std::vector<vertex> vertex) { p_vertex = vertex; }
        void setLiquid( bool set) { p_liquid = set; lua_liquid( p_id); }
        void setSolid( bool set) { p_solid = set; }
        void setGravity( bool set) { p_gravity = set; }
        bool getGravity() { return p_gravity; }

        void lua_action( std::string l_action) {
            if( p_state == NULL)
                return;

            // name the function
            lua_getglobal( p_state, l_action.c_str());
            if( !lua_isfunction( p_state, -1)) {
                lua_pop( p_state,1);
                return;
            }
            lua_pushnumber( p_state, this->getId());
            // call the function
            if( lua_pcall( p_state, 1, 0, 0))
                printf("entity::lua_action %s\n", lua_tostring( p_state, -1));
        }

        bool NeedUpdate() { return p_update; }

        void loadScript( std::string file);

        void lua_start( int id);
        void lua_vertexhit( int id);
        void lua_jump( int id);
        void lua_right( int id);
        void lua_left( int id);
        void lua_up( int id);
        void lua_down( int id);
        void lua_run( int id, bool press);
        void lua_attack( int id);
        void lua_special( int id);
        void lua_update( int id);
        void lua_collision( int id, std::vector<int> ids);
        void lua_liquid( int id);
        int lua_timer( int id, int time);

        void lua_printerror();
        bool lua_hasLoaded() { return p_state==NULL?false:true; }
        lua_State *lua_getLua() { return p_state; }


        int getId() { return p_id; }
        void addVelocity( fvec2 velocity) { p_velocity = p_velocity + velocity; }
        fvec2 getVelocity() { return p_velocity; }
        fvec2 getPosition() { return p_pos; }
        bool getVertexHit( int id);
        entitytype *getType() { return p_type; }
        void setColisionDown( bool set) { p_down = set; }
        void setColisionUp( bool set) { p_up = set; }
        void setColisionRight( bool set) { p_right = set; }
        void setColisionLeft( bool set) { p_left = set; }
        bool getColisionDown() { return p_down;}
        bool getColisionUp() { return p_up;}
        bool getColisionRight() { return p_right;}
        bool getColisionLeft() { return p_left;}
        bool isInLiquid() { return p_liquid;}
        bool isSolid() { return p_solid; }
        std::string getAction() { return p_action; }
        std::vector<vertex>* getVertex() { return &p_vertex; }
        timer *getTimer() { return &p_timer; }
        int isbedelete;
        bool isAlive() {
            if( getAction() == "die")
                return false;
            return true;
        }
        int getFrame() { return p_frame; }
        lua_State *getState() { return p_state; }
    protected:

    private:
        int p_id;
        fvec2 p_pos;
        std::string p_action;
        std::vector<vertex> p_vertex;
        entitytype *p_type;
        bool p_update;
        bool p_down;
        bool p_up;
        bool p_right;
        bool p_left;
        bool p_direction;
        fvec2 p_velocity;
        lua_State *p_state;
        timer p_timer;
        int p_timestartaction;
        int p_frame;
        bool p_liquid;
        bool p_solid;
        bool p_gravity;
};

class entitylist {
    public:
        entitylist();
        virtual ~entitylist();

        int create( entitytype *type, vec2 pos, int id = -1);
        void deleteObj( int id);
        std::vector<int> createFromWorldFile( std::string file, world *world);

        void draw(graphic *graphic, particle_list *particle,config *config);

        int setVertexHit( vertex *vertex, bool set);

        void process( world *world, config *config, int dt);

        void vertexHit( entity *entity, int vertexid);
        std::vector <int> collision_boundingBox( entity* checkentity);
        std::vector <int> collision_boundingBoxRect( fvec2 l_postion, fvec2 l_size, int id);
        bool loadType( std::string folder, graphic *graphic);
        void removeTypes();
        void loadTypes( std::string folder, graphic *graphic);
        entitytype *getType( std::string name);
        std::vector<int> findPlayerObject();
        void message( int id, std::string text);

        entity *getEntity( int id);
        int getAmountPlayerObject() { return p_playerentity; }
        std::vector<entity> getEntitys() { return p_entitys; }
        void clearEntitys( ) { p_entitys.clear(); }
        void setEntitys( std::vector<entity> entitys) { p_entitys = entitys; }
    protected:

    private:
        std::vector<entitytype> p_entity_types;
        std::vector<entity> p_entitys;
        std::vector<entity_text> p_text;
        int p_id;

        int p_playerentity;

        std::string p_folder;
};

void lua_setLink( entitylist *entity, world *world);


#endif // ENTITY_H
