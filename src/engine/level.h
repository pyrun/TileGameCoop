#ifndef LEVEL_H
#define LEVEL_H

#include <string>
#include "world.h"
#include "entity.h"
#include "../graphic/graphic.h"
#include "types.h"
#include "player.h"
#include "particle.h"
#include "transition.h"

#define transition_time 500

class level
{
    public:
        level(std::string file, std::string folder, graphic *graphic, player_handle *player, config *config, entitylist *t_entitylist = NULL);
        virtual ~level();

        void process( float l_delta, config *config, graphic *graphic, player_handle *playerlist, particle_list* particle);
        void draw( graphic* graphic);

        entitylist *getEntityList() {
            if( p_level != NULL)
                return p_level->getEntityList();
            return p_entity;
        }
        world *getWorld() {
            if( p_level != NULL)
                return p_level->getWorld();
            return p_world;
        }

        world *getWorldOnce() {
            return p_world;
        }
        /*world *getLevel() {
            if( p_level != NULL)
                return p_level->getWorld();
            return p_world;
        }*/
        void setLevel( level *level) {
            p_level = level;
        }

        level *getLevel() {
            return p_level;
        }
        bool isWorking() {
            printf( "%d %d\n", !p_loadworld, !p_transition);
            return (!p_loadworld && !p_transition);
        }
    protected:

    private:
        world *p_world;
        entitylist *p_entity;
        level *p_level = NULL;
        std::vector<entity> p_entityListSave;

        transition *p_transition;

        bool p_loadworld;
        bool p_notMyEntityList;

        fvec2 p_camere_pos;
};

#endif // LEVEL_H
