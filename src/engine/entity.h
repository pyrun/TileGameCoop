#ifndef ENTITY_H
#define ENTITY_H

#include <string>
#include <vector>

#include "types.h"
#include "../xml/tinyxml2.h"

#define ENTITY_FILE "definition.xml"

enum action_name { action_walk = 1, action_run, action_jump, action_swim, action_special, action_die};

class action
{
    public:

    protected:

    private:
};

class entitytype
{
    public:
        entitytype() {};
        virtual ~entitytype() {};

        int width;
        int height;
        std::string name;
    protected:

    private:
};

class entity
{
    public:
        entity();
        virtual ~entity();

        void setType( entitytype *type) { this->type = type; }
    protected:

    private:
        int p_id;

        entitytype *type;
};

class entitylist {
    public:
        entitylist();
        virtual ~entitylist();

        int create( entitytype *type, vec2 pos);

        bool loadType( std::string folder);
        void removeTypes();
        entitytype *getType( std::string name);
    protected:

    private:
        std::vector<entitytype> p_entity_types;
        std::vector<entity> p_entitys;
        int p_id;

        std::string p_folder;
};

#endif // ENTITY_H
