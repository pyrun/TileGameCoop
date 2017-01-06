#ifndef ENTITY_H
#define ENTITY_H

#include <string>

enum Action_name { action_walk = 1, action_run, action_jump, action_swim, action_special};

class Action
{

};

class Entity
{
    public:
        Entity();
        virtual ~Entity();

    protected:

    private:
};

class EntityList {
    public:
        EntityList();
        virtual ~EntityList();

        bool load( std::string folder);
    protected:

    private:
};

#endif // ENTITY_H
