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
    p_entity->createFromWorldFile( p_world->getFileName());

    // load all image files
    p_world->loadImageFiles( graphic);
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
