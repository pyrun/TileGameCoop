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

        void process( float l_delta, config *config, graphic *graphic, player_handle *playerlist) {
            // process entity
            getEntityList()->process( getWorld(), config, l_delta);

            // check if level finish
            if( p_level != NULL) {
                if( p_level->getWorld()->isLevelEnd() == true) {
                    // alle player daten aufnhemen auf die NEUE liste
                    if( p_level->getWorld()->leaveLevelasPlayer()){
                        std::vector<int> l_obj = p_level->getEntityList()->findPlayerObject();
                        for( int n = 0; n < (int)l_obj.size(); n++) {
                            entity *l_entity = p_level->getEntityList()->getEntity( l_obj[n]);
                            // ist es am leben
                            if( l_entity->isAlive()) {
                                playerlist->addEntity(l_entity->getType()->getName() );
                                printf( "add %s\n",  l_entity->getType()->getName().c_str());
                            }
                        }
                    }

                    delete p_level;
                    p_level = NULL;

                    // player reset
                    playerlist->setAllInavtive();

                    // set old link
                    lua_setLink( p_entity, p_world);
                }
            }

            if( p_level != NULL && !p_level->getWorld()->loadAsPlayer() && playerlist->getEntityList().size()) {
                std::vector<std::string> l_entity_names = playerlist->getEntityList();

                for( int i = 0; i < (int)l_entity_names.size(); i++) {
                    entitytype* l_type = getEntityList()->getType( l_entity_names[i]);
                    getEntityList()->create( l_type, getWorld()->getStartPoint());
                }
                playerlist->resetEntitys();
            }

            if( p_level == NULL && p_world->needLoadWorld() != "" ) {
                // player reset
                playerlist->setAllInavtive();

                std::string l_level = getWorld()->needLoadWorld();
                p_world->setLoadWorld( "", false); // NULL
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
