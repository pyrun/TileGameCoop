#ifndef ENTITY_H
#define ENTITY_H

#include <string>
#include <vector>
#include <lua.hpp>

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
        image *imagefile;
};

class vertex {
    public:
        vec2 pos;
        bool up;
        bool left;
        bool right;
        bool down;
};

class entitytype
{
    public:
        entitytype();
        virtual ~entitytype();

        action* getAction( std::string name);

        void addAction( std::string name, std::string file, int frame, int speed, image *image);
        void addVertex(vec2 pos, bool left, bool right, bool up, bool down);

        void loadScript( std::string file);

        void lua_jump( int id);
        void lua_right( int id);
        void lua_left( int id);
        void lua_up( int id);
        void lua_down( int id);

        void lua_printerror();
        bool lua_hasLoaded() { return p_state!=NULL?true:false; }

        void setName( std::string name) { p_name = name; }
        void setGravity( bool mass) { p_gravity = mass; }

        void setWidth( int width) { p_width = width; }
        void setHeight( int height) { p_height = height; }
        void setIsPlayer( bool isplayer) { p_isplayer = isplayer; }

        std::string getName() { return p_name; }
        int getWidth() { return p_width; }
        int getHeight() { return p_height; }
        bool getGravity() { return p_gravity; }
        std::vector<vertex> getVertex() { return p_vertex; }
        bool getIsPlayer() { return p_isplayer; }
    protected:

    private:
        int p_width;
        int p_height;
        bool p_gravity;
        bool p_isplayer;
        std::string p_name;
        std::vector<action> p_actions;
        std::vector<vertex> p_vertex;

        lua_State *p_state;
};

class entity
{
    public:
        entity( int id);
        virtual ~entity();

        void draw( graphic *graphic);

        void setType( entitytype *type) { this->p_type = type; }
        void setAction( std::string name) { p_action = name; }
        void setPos( vec2 pos) { p_pos = pos; }
        void setPos( fvec2 pos) { p_pos = pos; }
        void setVelocity( fvec2 velocity) { p_velocity = velocity; }

        int getId() { return p_id; }
        void addVelocity( fvec2 velocity) { p_velocity = p_velocity + velocity; }
        fvec2 getVelocity() { return p_velocity; }
        fvec2 getPosition() { return p_pos; }
        entitytype *getType() { return p_type; }
        void setColisionDown( bool set) { p_down = set; }
        void setColisionUp( bool set) { p_up = set; }
        void setColisionRight( bool set) { p_right = set; }
        void setColisionLeft( bool set) { p_left = set; }
        bool getColisionDown() { return p_down;}
        bool getColisionUp() { return p_up;}
        bool getColisionRight() { return p_right;}
        bool getColisionLeft() { return p_left;}
    protected:

    private:
        int p_id;
        fvec2 p_pos;
        std::string p_action;
        entitytype *p_type;
        bool p_down;
        bool p_up;
        bool p_right;
        bool p_left;


        fvec2 p_velocity;
};

class entitylist {
    public:
        entitylist();
        virtual ~entitylist();

        int create( entitytype *type, vec2 pos);
        void createFromWorldFile( std::string file);

        void draw(graphic *graphic);

        void process( world *world, int dt);

        bool loadType( std::string folder, graphic *graphic);
        void removeTypes();
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
