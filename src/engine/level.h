#ifndef LEVEL_H
#define LEVEL_H

#include <string>
#include "world.h"
#include "entity.h"
#include "../graphic/graphic.h"

class level
{
    public:
        level(std::string file, std::string folder, graphic *graphic);
        virtual ~level();

        entitylist *getEntityList() { return p_entity; }
        world *getWorld() { return p_world; }
    protected:

    private:
        world *p_world;
        entitylist *p_entity;
        level *p_level = NULL;
};

#endif // LEVEL_H
