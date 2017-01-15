#ifndef ENTITY_H
#define ENTITY_H

#include <string>
#include <vector>

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
        entitytype() { };
        virtual ~entitytype() {};

        action* getAction( std::string name);



        void addAction( std::string name, std::string file, int frame, int speed, image *image) {
            action *l_action = new action;

            l_action->name = name;
            l_action->file = file;
            l_action->frame = frame;
            l_action->speed = speed;
            l_action->imagefile = image;

            p_actions.push_back( *l_action);
        }
        void addVertex(vec2 pos, bool left, bool right, bool up, bool down) {
            vertex *l_vertex = new vertex;

            l_vertex->pos = pos;
            l_vertex->left = left;
            l_vertex->right = right;
            l_vertex->down = down;
            l_vertex->up = up;

            p_vertex.push_back( *l_vertex);
        }

        void setName( std::string name) { p_name = name; }
        void setGravity( bool mass) { p_gravity = mass; }

        void setWidth( int width) { p_width = width; }
        void setHeight( int height) { p_height = height; }

        std::string getName() { return p_name; }
        int getWidth() { return p_width; }
        int getHeight() { return p_height; }
        bool getGravity() { return p_gravity; }
        std::vector<vertex> getVertex() { return p_vertex; }
    protected:

    private:
        int p_width;
        int p_height;
        bool p_gravity;
        std::string p_name;
        std::vector<action> p_actions;
        std::vector<vertex> p_vertex;
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
    protected:

    private:
        int p_id;
        fvec2 p_pos;
        std::string p_action;
        entitytype *p_type;

        fvec2 p_velocity;
};

class entitylist {
    public:
        entitylist();
        virtual ~entitylist();

        int create( entitytype *type, vec2 pos);

        void draw(graphic *graphic);

        void process( world *world, int dt);

        bool loadType( std::string folder, graphic *graphic);
        void removeTypes();
        entitytype *getType( std::string name);

        entity *getEntity( int id);
    protected:

    private:
        std::vector<entitytype> p_entity_types;
        std::vector<entity> p_entitys;
        int p_id;

        std::string p_folder;
};

#endif // ENTITY_H
