#ifndef ENTITY_H
#define ENTITY_H

#include <string>
#include <vector>

#include "../graphic/graphic.h"
#include "types.h"
#include "../xml/tinyxml2.h"

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

class action
{
    public:
        action();
        virtual ~action();

        std::string name;
        std::string file;
        int frame;
        image *image;
    protected:

    private:
};

class entitytype
{
    public:
        entitytype() {};
        virtual ~entitytype() {};

        action* getAction( std::string name);

        int width;
        int height;
        std::string name;
        std::vector<action> *actions;
    protected:

    private:
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

        int getId() { return p_id; }
    protected:

    private:
        int p_id;
        vec2 p_pos;
        std::string p_action;

        entitytype *p_type;

};

class entitylist {
    public:
        entitylist();
        virtual ~entitylist();

        int create( entitytype *type, vec2 pos);

        void draw(graphic *graphic);

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
