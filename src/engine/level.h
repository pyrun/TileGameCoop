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

#include "../xml/tinyxml2.h"

#define transition_time 200

#define SAVE_FILE "savefile.xml"

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
        void setLevel( level *level) {
            p_level = level;
        }

        void setSave() { p_setSave = true; }
        void setLoad() { p_setLoad = true; }
        void fast_save( player_handle *player);
        void save( player_handle *player);
        void fast_load( player_handle *player);
        void load( player_handle *player, graphic *graphic);
    protected:

    private:
        world *p_world;
        entitylist *p_entity;
        level *p_level = NULL;
        std::vector<entity> p_entityListSave;

        transition *p_transition;

        bool p_loadworld;
        bool p_notMyEntityList;
        bool p_setLoad;
        bool p_setSave;

        fvec2 p_camere_pos;
};

void lua_level_setLink( level* level);

#endif // LEVEL_H
