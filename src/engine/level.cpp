#include "level.h"

level::level(std::string file, std::string folder, graphic *graphic)
{
    p_entity = NULL;
    p_world = NULL;

    // create entity list
    p_entity = new entitylist();

    p_entity->loadTypes( "creature/", graphic);

    // load world
    p_world = new world( file, folder);

    // set link
    lua_setLink( p_entity, p_world);

    // load now all entitys
    p_entity->createFromWorldFile( p_world->getFileName(), p_world);

    // load all image files
    p_world->loadImageFiles( graphic);

    // camera set at start point
    vec2 l_start = p_world->getStartPoint()-graphic->getCameraSize()/vec2(2, 2);
    graphic->setCamera(l_start);
}

void level::process( float l_delta, config *config, graphic *graphic, player_handle *playerlist) {
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
                        printf( "level::process add %s\n",  l_entity->getType()->getName().c_str());
                    }
                }
            }

            delete p_level;
            p_level = NULL;

            // player reset
            playerlist->setAllInavtive();

            // set old link
            lua_setLink( p_entity, p_world);

            // set old camera pos
            graphic->setCamera( p_camere_pos.tovec2());
            graphic->flyTo( p_camere_pos.tovec2());
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

        // save old cam position
        p_camere_pos = graphic->getCamera();

        std::string l_level = getWorld()->needLoadWorld();
        p_world->setLoadWorld( "", false); // NULL
        p_level = new level( l_level, "worlds/", graphic);
    }
}

level::~level()
{
    // delete entity
    if( p_entity)
        delete p_entity;
    // delete world if a there
    if( p_world)
        delete p_world;
}
