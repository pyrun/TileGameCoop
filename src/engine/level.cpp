#include "level.h"

using namespace tinyxml2;

#ifndef XMLCheckResult
	#define XMLCheckResult(a_eResult) if (a_eResult != XML_SUCCESS) { printf("Error: %i\n", a_eResult); return a_eResult; }
#endif

level *lua_level = NULL;

static int lua_save( lua_State *state) {
    lua_level->setSave();
    return 0;
}

static int lua_load( lua_State *state) {
    lua_level->setLoad();
    return 0;
}

void lua_level_install( lua_State *state) {
    // add all entity function ..
    lua_pushcfunction( state, lua_save);
    lua_setglobal( state, "savestate");

    lua_pushcfunction( state, lua_load);
    lua_setglobal( state, "loadstate");
}

void lua_level_setLink( level* level) {
    // set list
    lua_level = level;
}


level::level(std::string file, std::string folder, graphic *graphic, player_handle *player, config *config, audio* audio, entitylist *t_entitylist)
{
    p_entity = NULL;
    p_world = NULL;
    p_transition = NULL;

    p_setSave = false;
    p_setLoad = false;

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
    lua_setLink( p_entity, p_world, graphic);
    //lua_config_setLink( config);
    lua_player_setLink( player, p_entity);

    p_transition = new transition( graphic, transition_time, true);

    // load now all entitys
    std::vector<int> l_ids = p_entity->createFromWorldFile( p_world->getFile(), p_world);
    for( int i = 0; i < l_ids.size(); i++) {
        entity *l_entity = p_entity->getEntity( l_ids[i]);

        if( l_entity && l_entity->getType()->getScriptName().size() > 0) {
            lua_player_install( l_entity->getState());
            //lua_config_install( l_entity->getState());
            lua_level_install( l_entity->getState());

            // call once the timer script
            l_entity->lua_timer( l_entity->getId(), 1);
        }
    }

    // load all image files
    p_world->loadImageFiles( graphic);

    // camera set at start point
    vec2 l_start = p_world->getStartPoint();

    // set camera position to start
    graphic->setCamera(l_start);

    // play music
    audio->playMusic( p_world->getMusicFile());
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

void level::process( float l_delta, config *config, graphic *graphic, player_handle *playerlist, particle_list *particle, audio *audio) {
    // handle save
    if( p_setSave == true) {
        p_setSave = false;
        fast_save( playerlist);
    }
    // after save now load state
    if( p_setLoad == true) {
        p_setLoad = false;
        // reset particle system
        particle->clear();
        // load world
        fast_load( playerlist);
    }

    // world end transition
    if( p_loadworld == false && p_level != NULL && p_level->getWorldOnce()->isLevelEnd() == true ) {
        p_transition = p_transition = new transition( graphic, transition_time, false);
        p_loadworld = true;
        return;
    }

    // check if level finish
    if( p_level != NULL && p_level->getWorldOnce()->isLevelEnd() == true && p_transition == NULL) {
            p_loadworld = false;

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
            lua_setLink( p_entity, p_world, graphic);

            // set old camera pos
            graphic->setCamera( p_camere_pos.tovec2());
            graphic->flyTo( p_camere_pos.tovec2());
    }

    if( p_transition == NULL && p_level != NULL) {
        p_level->process( l_delta, config, graphic, playerlist, particle, audio);
        return;
    }
    // transition
    p_world->process( graphic);

    if( !p_loadworld && !p_transition ) {
        p_entity->process( p_world, config, l_delta);
    }

    // transition wolrd go in
    if( p_loadworld == false && p_level == NULL && p_world->needLoadWorld() != "") {
        p_transition = new transition( graphic, transition_time, false);
        p_loadworld = true;
        return;
    }

    // if transition cancel process
    if( p_transition)
        return;

    // load new world?
    if( p_level == NULL && p_world->needLoadWorld() != "" ) {
        p_loadworld = false;
        // player reset
        playerlist->setAllInavtive();

        // save old cam position
        p_camere_pos = graphic->getCamera();

        // save old entity's and clear the list
        p_entityListSave = p_entity->getEntitys();
        p_entity->clearEntitys();

        std::string l_level = p_world->needLoadWorld();
        p_level = new level( l_level, "worlds/", graphic, playerlist, config, audio, p_entity);

        //p_transition = new transition( graphic, transition_time, true)

        // transition
        getWorld()->process( graphic);

        // process entity
        getEntityList()->process( getWorld(), config, l_delta);

        // reset particle system
        particle->clear();
    }
}

void level::draw( graphic* graphic) {
    if( p_transition == NULL && p_level != NULL)
        p_level->draw( graphic);
    if( p_transition)
        if( p_transition->draw( graphic)) {
            delete p_transition;
            p_transition = NULL;
        }
}

void level::fast_save( player_handle *player) {
    XMLDocument l_savefile;

    // create root node
    XMLNode *l_root = l_savefile.NewElement("savefile");

    // insert the root node
    l_savefile.InsertFirstChild(l_root);

     // save player
    // creating root player element
    XMLElement *l_xmlPlayer = l_savefile.NewElement( "player");
    std::vector<player_data> l_playerdata = player->getPlayerData();

    // save every data string
    for( int i = 0; i < (int)l_playerdata.size();i++ ) {
        std::string l_player_index = l_playerdata[i].index;
        std::string l_player_data = l_playerdata[i].data;

        XMLElement *l_xmlPlayerData = l_savefile.NewElement( "entity");

        // index
        l_xmlPlayerData->SetAttribute( "index", l_player_index.c_str());
        l_xmlPlayerData->SetText( l_player_data.c_str());

        // save entity
        l_xmlPlayer->LinkEndChild( l_xmlPlayerData);
    }
    // link to the end
    l_root->LinkEndChild( l_xmlPlayer);

    // get World data
    XMLElement *l_xmlWorld = l_savefile.NewElement( "world");

    // save name and extras
    l_xmlWorld->SetAttribute( "file", getWorld()->getFileName().c_str());
    l_xmlWorld->SetAttribute( "width", getWorld()->getWorld().x);
    l_xmlWorld->SetAttribute( "height", getWorld()->getWorld().y);

    // link to the end
    l_root->LinkEndChild( l_xmlWorld);

    // save
    l_savefile.SaveFile( SAVE_FILE);

    printf( "level::fast_save world \"%s\" saved\n", getWorld()->getFileName().c_str());
}

void level::save( player_handle *player) {
    XMLDocument l_savefile;

    // create root node
    XMLNode *l_root = l_savefile.NewElement("savefile");

    // insert the root node
    l_savefile.InsertFirstChild(l_root);

    // save player
    // creating root player element
    XMLElement *l_xmlPlayer = l_savefile.NewElement( "player");
    std::vector<player_data> l_playerdata = player->getPlayerData();

    // save every data string
    for( int i = 0; i < (int)l_playerdata.size();i++ ) {
        std::string l_player_index = l_playerdata[i].index;
        std::string l_player_data = l_playerdata[i].data;

        XMLElement *l_xmlPlayerData = l_savefile.NewElement( "entity");

        // index
        l_xmlPlayerData->SetText( l_player_index.c_str());
        l_xmlPlayerData->SetAttribute( "data", l_player_data.c_str());

        // save entity
        l_xmlPlayer->LinkEndChild( l_xmlPlayerData);
    }
    // link to the end
    l_root->LinkEndChild( l_xmlPlayer);

    // get entity's
    std::vector<entity> l_entitys = p_entity->getEntitys();

    // creating root entity element
    XMLElement *l_xmlEntitys = l_savefile.NewElement( "entitys");

    // get every data from the entity
    for( int i = 0; i < (int)l_entitys.size();i++ ) {
        entity *l_obj = &l_entitys[i];
        XMLElement *l_xmlEntity = l_savefile.NewElement( "entity");

        // set name
        l_xmlEntity->SetText( l_obj->getType()->getName().c_str());

        // id
        l_xmlEntity->SetAttribute( "id", l_obj->getId());

        // position
        l_xmlEntity->SetAttribute( "positionX", l_obj->getPosition().tovec2().x);
        l_xmlEntity->SetAttribute( "positionY", l_obj->getPosition().tovec2().y);

        // direction
        l_xmlEntity->SetAttribute( "direction", l_obj->getDirection());

        // set global
        l_xmlEntity->SetAttribute( "global1", l_obj->getGlobal().c_str());
        l_xmlEntity->SetAttribute( "global2", l_obj->getGlobal2().c_str());
        l_xmlEntity->SetAttribute( "global3", l_obj->getGlobal3().c_str());

        // save entity
        l_xmlEntitys->LinkEndChild( l_xmlEntity);
    }

    // link to the end
    l_root->LinkEndChild( l_xmlEntitys);

    // get World data
    XMLElement *l_xmlWorld = l_savefile.NewElement( "world");

    // save name and extras
    l_xmlWorld->SetAttribute( "file", getWorld()->getFileName().c_str());
    l_xmlWorld->SetAttribute( "width", getWorld()->getWorld().x);
    l_xmlWorld->SetAttribute( "height", getWorld()->getWorld().y);

    // save every tile
    XMLElement *l_xmlOverlap = l_savefile.NewElement( "overlap");
    tile *l_map = getWorld()->getOverlap();
    // process every tile
    for( int i = 0; i < getWorld()->getWorld().x*getWorld()->getWorld().y; i++ ) {
        // tile
        tile *l_tile = &l_map[ i];
        XMLElement *l_xmlTile = l_savefile.NewElement( "tile");
        l_xmlTile->SetText( l_tile->id);
        l_xmlTile->SetAttribute( "x", l_tile->pos.x);
        l_xmlTile->SetAttribute( "y", l_tile->pos.y);
        l_xmlOverlap->LinkEndChild( l_xmlTile);
    }
    // link world map to the end
    l_xmlWorld->LinkEndChild( l_xmlOverlap);

    // save every tile
    XMLElement *l_xmlForground = l_savefile.NewElement( "foreground");
    l_map = getWorld()->getForeground();
    // process every tile
    for( int i = 0; i < getWorld()->getWorld().x*getWorld()->getWorld().y; i++ ) {
        // tile
        tile *l_tile = &l_map[ i];
        XMLElement *l_xmlTile = l_savefile.NewElement( "tile");
        l_xmlTile->SetText( l_tile->id);
        l_xmlTile->SetAttribute( "x", l_tile->pos.x);
        l_xmlTile->SetAttribute( "y", l_tile->pos.y);
        l_xmlForground->LinkEndChild( l_xmlTile);
    }
    // link world map to the end
    l_xmlWorld->LinkEndChild( l_xmlForground);

    // save every tile
    XMLElement *l_xmlBackground = l_savefile.NewElement( "background");
    l_map = getWorld()->getBackground();
    // process every tile
    for( int i = 0; i < getWorld()->getWorld().x*getWorld()->getWorld().y; i++ ) {
        // tile
        tile *l_tile = &l_map[ i];
        XMLElement *l_xmlTile = l_savefile.NewElement( "tile");
        l_xmlTile->SetText( l_tile->id);
        l_xmlTile->SetAttribute( "x", l_tile->pos.x);
        l_xmlTile->SetAttribute( "y", l_tile->pos.y);
        l_xmlBackground->LinkEndChild( l_xmlTile);
    }
    // link world map to the end
    l_xmlWorld->LinkEndChild( l_xmlBackground);

    // link to the end
    l_root->LinkEndChild( l_xmlWorld);

    // save
    l_savefile.SaveFile( SAVE_FILE);

}

void level::fast_load( player_handle *player) {
    XMLDocument l_file;

    // load the save file
    XMLError l_result = l_file.LoadFile( SAVE_FILE);

    // file exist?
    if( file_exist( SAVE_FILE) == false) {
        printf( "level::load file dont exist \"%s\"\n", SAVE_FILE);
        return;
    }

    XMLElement* l_xml_root = l_file.FirstChildElement( "savefile" );
    if( !l_xml_root) {
        printf( "level::load root xml element not found\n");
        return;
    }

    XMLElement* l_xml_player = l_xml_root->FirstChildElement( "player" );
    XMLElement* l_xml_player_entity = l_xml_player->FirstChildElement( "entity" );
    while( l_xml_player_entity) {
        std::string l_index;
        std::string l_data;

        // position
        l_index = l_xml_player_entity->Attribute( "index");
        l_data = l_xml_player_entity->GetText();

        // save to player
        player->setData( l_index, l_data);
        printf( "level::fast_load %s %s\n", l_index.c_str(), l_data.c_str());

        // next tile
        l_xml_player_entity = l_xml_player_entity->NextSiblingElement( "tile");
    }

    // load world
    XMLElement* l_xml_world = l_xml_root->FirstChildElement( "world" );
    if( !l_xml_world) {
        printf( "level::load world xml element not found\n");
        return;
    }

    std::string l_filename = l_xml_world->Attribute( "file");

    getWorld()->setLoadWorld( l_filename);
}

void level::load( player_handle *player, graphic *graphic) {
    XMLDocument l_file;

    // load the save file
    XMLError l_result = l_file.LoadFile( SAVE_FILE);

    // file exist?
    if( file_exist( SAVE_FILE) == false) {
        printf( "level::load file dont exist \"%s\"\n", SAVE_FILE);
        return;
    }

    XMLElement* l_xml_root = l_file.FirstChildElement( "savefile" );
    if( !l_xml_root) {
        printf( "level::load root xml element not found\n");
        return;
    }

    // todo
    XMLElement* l_xml_player = l_xml_root->FirstChildElement( "player" );
    if( !l_xml_player) {
        printf( "level::load player xml element not found\n");
        return;
    }


    // load world
    XMLElement* l_xml_world = l_xml_root->FirstChildElement( "world" );
    if( !l_xml_world) {
        printf( "level::load world xml element not found\n");
        return;
    }

    // create new world
    delete p_world;

    std::string l_filename = l_xml_world->Attribute( "file");
    p_world = new world( l_filename);
    lua_setLink( p_entity, p_world, graphic);
    // clear the entitys
    p_entity->clearEntitys();

    // set the tiles
    XMLElement* l_xml_overlap = l_xml_world->FirstChildElement( "overlap" );
    if( !l_xml_overlap) {
        printf( "level::load overlap xml element not found\n");
        return;
    }
    XMLElement* l_xml_tile = l_xml_overlap->FirstChildElement( "tile" );
    while( l_xml_tile) {
        int l_tile_id;
        vec2 l_tile_pos;

        // position
        l_tile_pos.x = atoi( l_xml_tile->Attribute( "x") );
        l_tile_pos.y = atoi( l_xml_tile->Attribute( "y") );

        // get id
        l_tile_id = atoi( l_xml_tile->GetText());

        // set tile
        p_world->getTile( p_world->getOverlap(), l_tile_pos)->id = l_tile_id;

        // next tile
        l_xml_tile = l_xml_tile->NextSiblingElement( "tile");
    }

    // set the tiles
    XMLElement* l_xml_foreground = l_xml_world->FirstChildElement( "foreground" );
    if( !l_xml_foreground) {
        printf( "level::load foreground xml element not found\n");
        return;
    }
    l_xml_tile = l_xml_foreground->FirstChildElement( "tile" );
    while( l_xml_tile) {
        int l_tile_id;
        vec2 l_tile_pos;

        // position
        l_tile_pos.x = atoi( l_xml_tile->Attribute( "x") );
        l_tile_pos.y = atoi( l_xml_tile->Attribute( "y") );

        // get id
        l_tile_id = atoi( l_xml_tile->GetText());

        // set tile
        p_world->getTile( p_world->getForeground(), l_tile_pos)->id = l_tile_id;

        // next tile
        l_xml_tile = l_xml_tile->NextSiblingElement( "tile");
    }

    // set the tiles
    XMLElement* l_xml_background = l_xml_world->FirstChildElement( "background" );
    if( !l_xml_background) {
        printf( "level::load background xml element not found\n");
        return;
    }
    l_xml_tile = l_xml_background->FirstChildElement( "tile" );
    while( l_xml_tile) {
        int l_tile_id;
        vec2 l_tile_pos;

        // position
        l_tile_pos.x = atoi( l_xml_tile->Attribute( "x") );
        l_tile_pos.y = atoi( l_xml_tile->Attribute( "y") );

        // get id
        l_tile_id = atoi( l_xml_tile->GetText());

        // set tile
        p_world->getTile( p_world->getBackground(), l_tile_pos)->id = l_tile_id;

        // next tile
        l_xml_tile = l_xml_tile->NextSiblingElement( "tile");
    }

    // entity
    XMLElement* l_xml_entitylist = l_xml_root->FirstChildElement( "entitys" );
    if( !l_xml_entitylist) {
        printf( "level::load entitys xml element not found\n");
        return;
    }
    XMLElement* l_xml_entity = l_xml_entitylist->FirstChildElement( "entity" );
    while( l_xml_entity) {
        std::string l_name;
        int l_id;
        vec2 l_entity_pos;
        bool l_direction;
        std::string l_global1;
        std::string l_global2;
        std::string l_global3;

        l_id = atoi( l_xml_entity->Attribute( "id") );

        // position
        l_entity_pos.x = atoi( l_xml_entity->Attribute( "positionX") );
        l_entity_pos.y = atoi( l_xml_entity->Attribute( "positionY") );

        // direction
        l_direction = atoi( l_xml_entity->Attribute( "direction") );

        // get globals
        l_global1 = l_xml_entity->Attribute( "global1");
        l_global2 = l_xml_entity->Attribute( "global2");
        l_global3 = l_xml_entity->Attribute( "global3");

        // get name
        l_name = l_xml_entity->GetText();

        // create the entity
        int l_entity_id = p_entity->create( p_entity->getType( l_name), l_entity_pos, l_id);
        entity *l_obj = p_entity->getEntity( l_entity_id);
        // set settings
        l_obj->setDirection( l_direction);
        if( l_global1 != "")
            l_obj->setGlobal( l_global1);
        if( l_global2 != "")
            l_obj->setGlobal2( l_global2);
        if( l_global3 != "")
            l_obj->setGlobal3( l_global3);

        // set link
        lua_player_install( l_obj->getState());
        //lua_config_install( l_obj->getState());
        lua_level_install( l_obj->getState());

        // next tile
        l_xml_entity = l_xml_entity->NextSiblingElement( "entity");
    }


}
