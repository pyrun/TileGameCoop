#ifndef ENTITY_H
#define ENTITY_H

#include <string>
#include <vector>

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

class action {
    public:
        std::string name;
        std::string file;
        int frame;
        int speed;
        int loop;
        image *imagefile;
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

class entitytype
{
    public:
        entitytype();
        virtual ~entitytype();

        action* getAction( std::string name);

        void addAction( std::string name, std::string file, int frame, int speed, int loop, image *image);
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
        void setAction( std::string name) { if( p_action != name) { p_action = name; p_timestartaction = -1;} }
        void setDirection( bool dir) { p_direction = dir; }
        int getDirection() { return p_direction; }
        void setPos( vec2 pos) { p_pos = pos; }
        void setPos( fvec2 pos) { p_pos = pos; }
        void setVelocity( fvec2 velocity) { p_velocity = velocity; }
        void setUpdate( bool set) { p_update = set; }
        void setVertex( std::vector<vertex> vertex) { p_vertex = vertex; }
        void setLiquid( bool set) { p_liquid = set; lua_liquid( p_id); }
        void setSolid( bool set) { p_solid = set; }

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
        int p_actionframe;
        int p_timestartaction;
        bool p_liquid;
        bool p_solid;
};

class entitylist {
    public:
        entitylist();
        virtual ~entitylist();

        int create( entitytype *type, vec2 pos);
        void deleteObj( int id);
        void createFromWorldFile( std::string file);

        void draw(graphic *graphic);

        int setVertexHit( vertex *vertex, bool set);

        void process( world *world, int dt);

        void vertexHit( entity *entity, int vertexid);
        std::vector <int> collision_boundingBox( entity* checkentity);
        bool loadType( std::string folder, graphic *graphic);
        void removeTypes();
        void loadTypes( std::string folder, graphic *graphic);
        entitytype *getType( std::string name);
        std::vector<int> findPlayerObject();

        entity *getEntity( int id);
        int getAmountPlayerObject() { return p_playerentity; }
    protected:

    private:
        std::vector<entitytype> p_entity_types;
        std::vector<entity> p_entitys;
        int p_id;

        int p_playerentity;

        std::string p_folder;
};

#endif // ENTITY_H
