void level::process( float l_delta, config *config, graphic *graphic, player_handle *playerlist, particle_list *particle) {
    //printf( "%s ", this->getWorld()->getFileName().c_str());

    if( p_level != NULL && p_level->getLevel() != NULL ) {
        p_level->process( l_delta, config, graphic, playerlist, particle);
        return;
    }
    //printf( "\n");

    // transition
    getWorld()->process( graphic);
    getEntityList()->process( getWorld(), config, l_delta);

    if( !p_loadworld && !p_transition ) {
        // process entity
        
    }

    // if transition cancel process
    if( p_transition)
        return;

    // transition wolrd go in
    if( p_loadworld == false && p_level == NULL && p_world->needLoadWorld() != "" ) {
        p_transition = p_transition = new transition( graphic, transition_time, false);
        p_loadworld = true;
        return;
    }

    // world end transition
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
        p_level = new level( l_level, "worlds/", graphic, playerlist, config, p_entity);

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

