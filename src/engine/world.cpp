#include "world.h"

using namespace tinyxml2;

#ifndef XMLCheckResult
	#define XMLCheckResult(a_eResult) if (a_eResult != XML_SUCCESS) { printf("Error: %i\n", a_eResult); return a_eResult; }
#endif

world::world(std::string file = "default.tmx", std::string ordner = "worlds/")
{
    // tilemap
    p_tilemap_overlap = NULL;
    p_tilemap_foreground = NULL;
    p_tilemap_background = NULL;
    p_tileset = NULL;

    p_file = file;
    if(!load( file, ordner))
        printf( "world::world cant load the file\n");
}

world::~world()
{
    if( p_tilemap_overlap)
        delete p_tilemap_overlap;
    if( p_tilemap_foreground)
        delete p_tilemap_foreground;
    if( p_tilemap_background)
        delete p_tilemap_background;
    //dtor
}

bool world::load( std::string file, std::string ordner) {
    XMLDocument l_file;

    int l_map_width;
    int l_map_height;

    int l_tilewidth;
    int l_tilehight;

    std::string l_tileset;
    int l_tileset_width;
    int l_tileset_height;

    std::string l_overlap;
    std::string l_foreground;
    std::string l_background;

    // file set
    if(file != "")
        p_file = ordner+file;

    // load the file
    XMLError l_result = l_file.LoadFile( p_file.c_str());

    // check the file
    XMLCheckResult(l_result);

    // map
    XMLElement* l_map = l_file.FirstChildElement( "map" );
    if( !l_map)
        return false;
    l_map_width = atoi(l_map->Attribute( "width" ));
    l_map_height = atoi( l_map->Attribute( "height" ));

    // tile
    XMLElement* l_xml_tileset = l_map->FirstChildElement( "tileset" );
    if( !l_xml_tileset)
        return false;
    l_tilewidth = atoi(l_xml_tileset->Attribute( "tilewidth" ));
    l_tilehight = atoi(l_xml_tileset->Attribute( "tileheight" ));
    XMLElement* l_xml_tileset_source = l_xml_tileset->FirstChildElement( "image" );
    if( !l_xml_tileset_source)
        return false;
    l_tileset = l_xml_tileset_source->Attribute( "source" );
    l_tileset_width = atoi(l_xml_tileset_source->Attribute( "width" ));
    l_tileset_height = atoi(l_xml_tileset_source->Attribute( "height"));

    // data
    XMLElement* l_layer = l_map->FirstChildElement( "layer" );
    if( !l_layer)
        return false;

    l_background = l_layer->FirstChildElement("data")->GetText();
    l_layer = l_layer->NextSiblingElement("layer");
    if( !l_layer)
        return false;

    l_foreground = l_layer->FirstChildElement("data")->GetText();
    l_layer = l_layer->NextSiblingElement("layer");
    if( !l_layer)
        return false;

    l_overlap = l_layer->FirstChildElement("data")->GetText();

    // image
    XMLElement* l_background_element = l_map->FirstChildElement( "imagelayer");
    while( l_background_element != NULL) {
        addBackground( l_background_element, ordner);
        l_background_element = l_background_element->NextSiblingElement( "imagelayer");
    }
    printf( "backgrounds %d\n", p_backgrounds.size());

    // printf( "map %d %d\n", l_map_width, l_map_height);
    // printf( "tile %d %d \"%s\"\n", l_tilewidth, l_tilehight, l_tileset.c_str());
    // printf( "Data %s %s %s\n", l_overlap.c_str(), l_foreground.c_str(), l_background.c_str());

    //
    p_map_width = l_map_width;
    p_map_hight = l_map_height;

    // creating world
    p_tilemap_overlap = readTilemap( l_overlap);
    p_tilemap_foreground = readTilemap( l_foreground);
    p_tilemap_background = readTilemap( l_background);

    // safe tileset path
    p_tilesetpath = ordner + l_tileset;
    p_tileset_width = l_tileset_width;
    p_tileset_height = l_tileset_height;

    p_tilewidth = l_tilewidth;
    p_tilehight = l_tilehight;

    printf( "Tileset %s %d %d\n", p_tilesetpath.c_str(), p_tileset_width/p_tilewidth, l_tileset_height/l_tilehight);

    return true;
}

tile *world::getTile( tile *tilemap, int x, int y) {
    tile *l_tile = NULL;
    if( x < p_map_width && y < p_map_hight)
        if( x >= 0 && y >= 0)
            l_tile = &tilemap[ y * p_map_width + x];
    return l_tile;
}

void world::addBackground( XMLElement* background, std::string ordner) {
    XMLElement* image;
    world_background l_background;
    image = NULL;

    // load the first child image
    image = background->FirstChildElement("image");
    // if didnt have just say i cant load it - not to bad
    if( !image ) {
        printf( "background not right defined\n");
        return;
    }
    // set the background data
    l_background.name = background->Attribute( "name" );
    l_background.picture_file = ordner + image->Attribute( "source");

    // add to backgrounds
    p_backgrounds.push_back( l_background);
}

void world::drawTile( graphic *graphic, int x, int y, tile *map) {
    // get the tile
    tile *l_tile = getTile( map, x, y);
    if( l_tile == NULL)
        return;

    int l_x = 0;
    l_x = l_tile->id;
    int l_y = 0;
    while( l_x > p_tileset_width/p_tilewidth) {
        l_x = l_x - (p_tileset_width/p_tilewidth);
        l_y = l_y +1;
    }

    if( l_x == 0)
        return;
    l_x -= 1;

    graphic->drawImage( p_tileset, vec2( x*p_tilewidth, y*p_tilewidth), vec2( p_tilewidth, p_tilehight), vec2( l_x*p_tilewidth, l_y*p_tilehight));
}

void world::drawBackAndForeground( graphic *graphic) {
    // load tileset if not loadead
    if( p_tileset == NULL) {
        p_tileset = graphic->loadImage( p_tilesetpath);
    } else {
        int l_camera_x = graphic->getCamera().x/p_tilewidth;
        int l_camera_y = graphic->getCamera().y/p_tilehight;

        int l_max_x = graphic->getCameraSize().x/p_tilewidth;
        int l_max_y = graphic->getCameraSize().y/p_tilehight;

        //
        for( int l_x = l_camera_x; l_x <= l_camera_x+l_max_x; l_x++) {
            for( int l_y = l_camera_y; l_y <= l_camera_y+l_max_y; l_y++) {
                drawTile( graphic, l_x, l_y, p_tilemap_background);
                drawTile( graphic, l_x, l_y, p_tilemap_foreground);
            }
        }
    }
}

void world::drawOverground( graphic *graphic) {
    // load tileset if not loadead
    if( p_tileset == NULL) {
        p_tileset = graphic->loadImage( p_tilesetpath);
    } else {
        int l_camera_x = graphic->getCamera().x/p_tilewidth;
        int l_camera_y = graphic->getCamera().y/p_tilehight;

        int l_max_x = graphic->getCameraSize().x/p_tilewidth;
        int l_max_y = graphic->getCameraSize().y/p_tilehight;

        //
        for( int l_x = l_camera_x; l_x <= l_camera_x+l_max_x; l_x++) {
            for( int l_y = l_camera_y; l_y <= l_camera_y+l_max_y; l_y++) {
                drawTile( graphic, l_x, l_y, p_tilemap_overlap);
            }
        }
    }
}

void world::draw( graphic *graphic) {
    if( (int)p_backgrounds.size() > 0) {
        if(p_backgrounds[0].picture == NULL)
            p_backgrounds[0].picture = graphic->loadImage( p_backgrounds[0].picture_file);
        else {
            world_background *l_background = &p_backgrounds[0];
            float l_factor = (float)l_background->picture->surface->h/ ((float)p_map_hight * (float)p_tilehight);

            vec2 l_position = { graphic->getCamera().x*0.25, graphic->getCamera().y*0.25};

            int l_background_x = 0;
            while(l_position.x-graphic->getCamera().x+ l_background->picture->surface->w/l_factor< 0)
                l_position.x+=l_background->picture->surface->w/l_factor;

            graphic->drawImage( l_background->picture, l_position, vec2( p_map_width * p_tilewidth, p_map_hight * p_tilehight), vec2( 0, 0), 0.0, 0, l_factor);

            graphic->drawImage( l_background->picture, l_position + vec2( l_background->picture->surface->w/l_factor, 0), vec2( p_map_width * p_tilewidth, p_map_hight * p_tilehight), vec2( 0, 0), 0.0, 0, l_factor);

            graphic->drawImage( l_background->picture, l_position + vec2( l_background->picture->surface->w/l_factor, 0) + vec2( l_background->picture->surface->w/l_factor, 0), vec2( p_map_width * p_tilewidth, p_map_hight * p_tilehight), vec2( 0, 0), 0.0, 0, l_factor);


        }
    }

    // load tileset if not loadead
    drawBackAndForeground( graphic);

    drawOverground( graphic);
}

tile *world::readTilemap( std::string tilemap) {
    tile *l_tilemap = new tile[ p_map_width * p_map_hight];
    char *l_input = (char *)tilemap.c_str();
    char *l_data;
    int l_amount = 0;
    // read the first data out
    l_data = strtok( l_input, ",");
    // now do this in a while - stop if no more data is
    while( l_data) {
        // creating tile
        tile l_tile;
        // set data
        l_tile.id = atoi( l_data);
        // save to the array
        l_tilemap[ l_amount] = l_tile;

        // next data read
        l_data = strtok( NULL, ",");

        // increase amount
        l_amount ++;
    }
    return l_tilemap;
}

std::string world::loadParameter( XMLNode *root, std::string name) {
    XMLElement * l_element = root->FirstChildElement( name.c_str());
    if (l_element == nullptr) return "";

    const char *l_out;
    l_out = l_element->Value();

    return l_out;
}
