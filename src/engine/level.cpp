#include "level.h"

level::level(std::string file, std::string folder, graphic *graphic, player_handle *player, entitylist *t_entitylist)
{
    p_entity = NULL;
    p_world = NULL;
    p_transition = NULL;

    p_loadworld = false;

    // create entity list if we have no
    if( t_entitylist == NULL) {
        p_entity = new entitylist();

        p_entity->loadTypes( "creature/", graphic);

        p_notMyEntityList = false;
    } else {
        p_entity = t_entitylist;
        // we become a clear list :) thanks
        p_notMyEntityList = true;
    }

    // load world
    p_world = new world( file, folder);

    // set link
    lua_setLink( p_entity, p_world);
    lua_player_setLink( player);

    p_transition = new transition( graphic, transition_time, true);

    // load now all entitys
    std::vector<int> l_ids = p_entity->createFromWorldFile( p_world->getFileName(), p_world);
    for( int i = 0; i < l_ids.size(); i++) {
        entity *l_entity = p_entity->getEntity( l_ids[i]);
        if( l_entity)
            lua_player_install( l_entity->getState());
    }

    // load all image files
    p_world->loadImageFiles( graphic);

    // camera set at start point
    vec2 l_start = p_world->getStartPoint()-graphic->getCameraSize()/vec2(2, 2);

    //
    graphic->setCamera(l_start);
}

level::~level()
{
    // delete entity
    if( p_entity && !p_notMyEntityList)
        delete p_entity;
    // delete world if a there
    if( p_world)
        delete p_world;
}

void level::process( float l_delta, config *config, graphic *graphic, player_handle *playerlist, particle_list *particle) {
    // transition
    getWorld()->process( graphic);

    if( !p_loadworld || ( p_transition && p_transition->blendout()) ) {
        // process entity
        getEntityList()->process( getWorld(), config, l_delta);
    }

    // if transition cancel process
    if( p_transition)
        return;

    if( p_loadworld == false && p_level == NULL && p_world->needLoadWorld() != "" ) {
        p_transition = p_transition = new transition( graphic, transition_time, false);
        p_loadworld = true;
        return;
    }

    if( p_loadworld == false && p_level != NULL && p_level->getWorld()->isLevelEnd() == true ) {
        p_transition = p_transition = new transition( graphic, transition_time, false);
        p_loadworld = true;
        return;
    }

     // check if level finish
    if( p_level != NULL && p_level->getWorld()->isLevelEnd() == true) {
            p_loadworld = false;

            // alle player daten aufnhemen auf die neue liste
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

            // create a transition
            p_transition = new transition( graphic, transition_time, true);

            // delete level
            delete p_level;
            p_level = NULL;

            // clear old list and load list from this level
            p_entity->clearEntitys();
            p_entity->setEntitys( p_entityListSave);

            // player reset
            playerlist->setAllInavtive();

            // set old link
            lua_setLink( p_entity, p_world);

            // set old camera pos
            graphic->setCamera( p_camere_pos.tovec2());
            graphic->flyTo( p_camere_pos.tovec2());
    }

    // load new world?
    if( p_level == NULL && p_world->needLoadWorld() != "" ) {
        p_loadworld = false;
        // player reset
        playerlist->setAllInavtive();

        // save old cam position
        p_camere_pos = graphic->getCamera();

        // save old entity's and clear the list
        p_entityListSave = getEntityList()->getEntitys();
        getEntityList()->clearEntitys();

        std::string l_level = getWorld()->needLoadWorld();
        bool l_loadAsPlayer = getWorld()->loadAsPlayer();
        p_world->setLoadWorld( "", false); // NULL
        p_level = new level( l_level, "worlds/", graphic, playerlist, p_entity );

        p_transition = new transition( graphic, transition_time, true);

        if( l_loadAsPlayer )
            playerlist->createChamps( getEntityList(), getWorld()->getStartPoint());
        printf( "getWorld()->loadAsPlayer() %d\n" ,l_loadAsPlayer);

        // transition
        getWorld()->process( graphic);

        // process entity
        getEntityList()->process( getWorld(), config, l_delta);

        // reset particle system
        particle->clear();
    }
}

void level::draw( graphic* graphic) {
    if( p_transition)
        if( p_transition->draw( graphic)) {
            delete p_transition;
            p_transition = NULL;
        }
}
