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

        void process( float l_delta, graphic *graphic) {
            // process entity
            getEntityList()->process( getWorld(), l_delta);

            // check if level finish
            if( p_level != NULL) {
                if( p_level->getWorld()->isLevelEnd() == true) {
                    delete p_level;
                    p_level = NULL;

                    // set old link
                    lua_setLink( p_entity, p_world);
                }
            }

            if( p_level == NULL && p_world->needLoadWorld() != "" ) {
                std::string l_level = getWorld()->needLoadWorld();
                p_world->setLoadWorld( ""); // NULL
                p_level = new level( l_level, "worlds/", graphic);
            }
        }

        entitylist *getEntityList() {
            if( p_level != NULL)
                return p_level->p_entity;
            return p_entity;
        }
        world *getWorld() {
            if( p_level != NULL)
                return p_level->p_world;
            return p_world;
        }
        void setLevel( level *level) {
            p_level = level;
        }
    protected:

    private:
        world *p_world;
        entitylist *p_entity;
        level *p_level = NULL;
};

#endif // LEVEL_H
