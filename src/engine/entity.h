#ifndef ENTITY_H
#define ENTITY_H

#include <string>
#include <vector>

#include "types.h"

enum Action_name { action_walk = 1, action_run, action_jump, action_swim, action_special, action_die};

class Action
{
    public:

    protected:

    private:
};

class Entity
{
    public:
        Entity();
        virtual ~Entity();

    protected:

    private:
};

class EntityType
{
    public:
        EntityType();
        virtual ~EntityType();

    protected:

    private:
};

class EntityList {
    public:
        EntityList();
        virtual ~EntityList();

        int create( EntityType obj, vec2 pos);

        bool loadType( std::string folder);
        void removeTypes();
    protected:

    private:
        std::vector<EntityType> p_entity_types;
        std::vector<Entity> p_entitys;
        int p_id;
};

#endif // ENTITY_H
