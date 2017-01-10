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

class action {
    public:
        std::string name;
        std::string file;
        int frame;
        int speed;
        image *image;
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
            l_action->image = image;

            p_actions.push_back( *l_action);
        }

        void setName( std::string name) { p_name = name; }

        void setWidth( int width) { p_width = width; }
        void setHeight( int height) { p_height = height; }

        std::string getName() { return p_name; }
        int getWidth() { return p_width; }
        int getHeight() { return p_height; }
    protected:

    private:
        int p_width;
        int p_height;
        std::string p_name;
        std::vector<action> p_actions;
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
