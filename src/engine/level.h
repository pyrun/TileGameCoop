#ifndef LEVEL_H
#define LEVEL_H

#include <string>
#include "world.h"
#include "entity.h"
#include "../graphic/graphic.h"
#include "types.h"
#include "player.h"

class level
{
    public:
        level(std::string file, std::string folder, graphic *graphic);
        virtual ~level();

        void process( float l_delta, config *config, graphic *graphic, player_handle *playerlist);

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

        fvec2 p_camere_pos;
};

#endif // LEVEL_H
