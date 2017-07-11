#include "world.h"

#include <math.h>

using namespace tinyxml2;

#ifndef XMLCheckResult
	#define XMLCheckResult(a_eResult) if (a_eResult != XML_SUCCESS) { printf("Error: %i\n", a_eResult); return a_eResult; }
#endif

world::world(std::string file, std::string ordner)
{
    p_level_end = false;

    p_needloadworld = "";

    // tilemap
    p_tilemap_overlap = NULL;
    p_tilemap_foreground = NULL;
    p_tilemap_background = NULL;
    p_tilemap_floorground = NULL;

    p_tileset = NULL;

    p_filename = file;
    if(!load( file, ordner))
        printf( "world::world cant load the file\n");

    // gravity
    p_gravity = 0.001f;
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

tile *world::getCollisionTileY( fvec2 position, fvec2 change, fvec2 velocity, bool up) {
    int l_tempy;
    int l_x = ( position.x )/p_tilewidth;
    int l_y = ( position.y )/p_tilehight;
    tile *l_tile = NULL;

    // border
    if( l_x < 0)
        return NULL;

    // change in steps annähern
    for( float i = 0; i < fabs(change.y); i+= 0.1f) {
        l_x = ( position.x )/p_tilewidth;
        l_y = ( position.y + i)/p_tilehight;

        if( up)
            l_tempy = l_y - 1;
        else
            l_tempy = l_y;

        // collision tile
        l_tile = getTile( p_tilemap_foreground, vec2( l_x, l_tempy) );
        if( l_tile == NULL)
            continue;

        // if air contiinue
        if( l_tile != NULL && l_tile->id == 0) {
            l_tile = NULL;
            continue;
        }
        if( l_tile->type != NULL && change.y > 0 )
            if( l_tile->type->top == 0) {
                l_tile = NULL;
                continue;
            }
        if( l_tile->type != NULL && change.y < 0 )
            if( l_tile->type->down == 0) {
                l_tile = NULL;
                continue;
            }
        break;
    }

    return l_tile;
}

tile *world::getCollisionTileX( fvec2 position, fvec2 change, fvec2 velocity) {
    int l_tempx;
    int l_x = ( position.x )/p_tilewidth;
    int l_y = ( position.y )/p_tilehight;
    tile *l_tile = NULL;

    // border
    if( position.x < 0)
        return NULL;

    // change in steps annähern
    for( float i = -2; i < fabs(change.x); i+= 0.5f) {
        l_x = ( position.x + i)/p_tilewidth;
        l_y = ( position.y )/p_tilehight;

        l_tempx = l_x;

        // collision tile
        l_tile = getTile( p_tilemap_foreground, vec2( l_tempx, l_y) );
        if( l_tile == NULL)
            continue;

        // if air contiinue
        if( l_tile != NULL && l_tile->id == 0) {
            l_tile = NULL;
            continue;
        }
        if( l_tile->type != NULL && change.y > 0 )
            if( l_tile->type->top == 0) {
                l_tile = NULL;
                continue;
            }
        if( l_tile->type != NULL && change.y < 0 )
            if( l_tile->type->down == 0) {
                l_tile = NULL;
                continue;
            }
        break;
    }

    return l_tile;
}

int world::loadTypes( std::string file) {
    XMLDocument l_file;
    XMLElement* l_tile;

    std::string l_xmlfile = file + ".xml";

    // if fille dont found
    if( file_exist( l_xmlfile) == false)
        return 1;

    // load the file
    XMLError l_result = l_file.LoadFile( l_xmlfile.c_str());
    XMLCheckResult(l_result);

    l_tile = l_file.FirstChildElement( "animation");
    while( l_tile) {
        tiletype l_type;
        XMLElement* l_animation_tile;

        if( l_tile->Attribute( "speed"))
            l_type.speed = atoi(l_tile->Attribute( "speed"));
        else
            l_type.speed = 0;

        // vor definierte werte
        l_type.down = true;
        l_type.top = true;
        l_type.right = true;
        l_type.left = true;
        l_type.liquid = false;

        if( l_tile->Attribute( "down"))
            l_type.down = atoi(l_tile->Attribute( "down"))==1?true:false;
        if( l_tile->Attribute( "up"))
            l_type.top = atoi(l_tile->Attribute( "up"))==1?true:false;
        if( l_tile->Attribute( "right"))
            l_type.right = atoi(l_tile->Attribute( "right"))==1?true:false;
        if( l_tile->Attribute( "left"))
            l_type.left = atoi(l_tile->Attribute( "left"))==1?true:false;
        if( l_tile->Attribute( "liquid"))
            l_type.liquid = atoi(l_tile->Attribute( "liquid"))==1?true:false;

        l_animation_tile = l_tile->FirstChildElement("tile");
        while( l_animation_tile) {
            l_type.id.push_back( atoi(l_animation_tile->GetText()) );
            l_animation_tile = l_animation_tile->NextSiblingElement("tile");
        }

        p_tiletypes.push_back( l_type);

        l_tile = l_tile->NextSiblingElement("animation");
    }

    return 0;
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
    int l_tileset_space;

    std::string l_overlap;
    std::string l_foreground;
    std::string l_background;
    std::string l_floorground;

    std::string l_map_temp;

    // file set
    if(file != "")
        p_file = ordner+file;

    // load the file
    XMLError l_result = l_file.LoadFile( p_file.c_str());
    if( file_exist( p_file) == false) {
        printf( "world::load file dont exist \"%s\"\n", p_file.c_str());
        return false;
    }

    // check the file
    XMLCheckResult(l_result);

    // map
    XMLElement* l_map = l_file.FirstChildElement( "map" );
    if( !l_map) {
        printf( "world::load map not defined - file corrupt? \n");
        return false;
    }
    l_map_width = atoi(l_map->Attribute( "width" ));
    l_map_height = atoi( l_map->Attribute( "height" ));

    // data
    XMLElement* l_properties = l_map->FirstChildElement( "properties" );
    if( !l_properties) {
        p_depthSort = false;
        printf( "world::load no properties found - warn\n");
    } else {
        // get all propetys
        XMLElement* l_property = l_properties->FirstChildElement( "property" );
        while( l_property) {
            std::string l_name =  l_property->Attribute( "name" );

            if( l_name == "depth_sort")
                p_depthSort = atoi( l_property->Attribute( "value" ));

            // next layer
            l_property = l_property->NextSiblingElement("property");
        }
    }

    // tile
    XMLElement* l_xml_tileset = l_map->FirstChildElement( "tileset" );
    if( !l_xml_tileset) {
        printf( "world::load tileset not defined in world file - cancel load\n");
        return false;
    }
    l_tilewidth = atoi(l_xml_tileset->Attribute( "tilewidth" ));
    l_tilehight = atoi(l_xml_tileset->Attribute( "tileheight" ));
    l_tileset_space = atoi(l_xml_tileset->Attribute( "spacing" ));

    XMLElement* l_xml_tileset_source = l_xml_tileset->FirstChildElement( "image" );
    if( !l_xml_tileset_source) {
        printf( "world::load tileset not defined in world file - cancel load\n");
        return false;
    }
    l_tileset = l_xml_tileset_source->Attribute( "source" );
    l_tileset_width = atoi(l_xml_tileset_source->Attribute( "width" ));
    l_tileset_height = atoi(l_xml_tileset_source->Attribute( "height"));

    // data
    XMLElement* l_layer = l_map->FirstChildElement( "layer" );
    if( !l_layer) {
        printf( "world::load no layer found - cancel load\n");
        return false;
    }
    // alle layer lesen
    while( l_layer) {
        l_map_temp = l_layer->FirstChildElement("data")->GetText();
        std::string l_name = l_layer->Attribute( "name" );

        // background target
        if( l_name == "Background" )
            l_background = l_map_temp;
        if( l_name == "Foreground" )
            l_foreground = l_map_temp;
        if( l_name == "Overlap" )
            l_overlap = l_map_temp;
        if( l_name == "Floorground" )
            l_floorground = l_map_temp;

        // next layer
        l_layer = l_layer->NextSiblingElement("layer");
    }

    // image
    XMLElement* l_background_element = l_map->FirstChildElement( "imagelayer");
    while( l_background_element != NULL) {
        addBackground( l_background_element, ordner);
        l_background_element = l_background_element->NextSiblingElement( "imagelayer");
    }

    // check if all world data here
    if( !l_overlap.size() || !l_background.size() || !l_foreground.size()) {
        printf( "world cant be loaded - world data \"Background\", \"Foreground\", or \"Overlap\" missing\n");
    }

    // map size save
    p_map_width = l_map_width;
    p_map_hight = l_map_height;
    p_tileset_space = l_tileset_space;

    // load types
    loadTypes( ordner + l_tileset);

    // creating world
    p_tilemap_overlap = readTilemap( l_overlap);
    p_tilemap_foreground = readTilemap( l_foreground);
    p_tilemap_background = readTilemap( l_background);

    // set floor ground
    if( l_floorground.size() > 0)
        p_tilemap_floorground = readTilemap( l_floorground);

    // safe tile set path
    p_tilesetpath = ordner + l_tileset;
    p_tileset_width = l_tileset_width;
    p_tileset_height = l_tileset_height;

    // save tile size
    p_tilewidth = l_tilewidth;
    p_tilehight = l_tilehight;
    return true;
}

tiletype *world::findType( int id) {
    for( int i = 0; i < (int)p_tiletypes.size(); i++) {
        for( int n = 0; n < (int)p_tiletypes[i].id.size(); n++)
            if( p_tiletypes[i].id[n] == id) {
                return &p_tiletypes[i];
            }
    }
    return NULL;
}

tile *world::getTile( tile *tilemap, vec2 pos) {
    // nicht über rand
    if( pos.x >= p_map_width || pos.y >= p_map_hight )
        return NULL;

    // keine negative werte
    if( pos.x < 0 || pos.y < 0)
        return NULL;


    return &tilemap[ pos.y * p_map_width + pos.x];
}

int world::getTypeIndex( int id, tiletype *type) {
    for( int i = 0; i < (int)type->id.size(); i++)
        if( type->id[i] == id-1)
            return i;
    return 0;
}

void world::addBackground( XMLElement* background, std::string ordner) {
    XMLElement* image;
    world_background l_background;
    image = NULL;


    // load the first child image
    image = background->FirstChildElement("image");
    // if didnt have just say i cant load it - not to bad
    if( !image ) {
        printf( "world::addBackground background not right defined\n");
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
    tile *l_tile = getTile( map, vec2(x, y) );
    if( l_tile == NULL)
        return;

    int l_x = 0;

    // Animation
    if( l_tile->type != NULL && l_tile->type->speed != 0)
            l_x = l_tile->type->id[ ((graphic->getFrame()/l_tile->type->speed) + getTypeIndex(l_tile->id, l_tile->type) )%l_tile->type->id.size()]+1;
    else
        l_x = l_tile->id;
    int l_y = 0;
    float l_factor = (float)p_tileset_width/(p_tilewidth+p_tileset_space);
    while( (float)(l_x > l_factor+0.5f) ) {
        l_x = l_x - (l_factor);
        l_y = l_y +1;
    }

    if( l_x == 0)
        return;
    l_x -= 1;

    graphic->drawImage( p_tileset, vec2( x*p_tilewidth, y*p_tilewidth), vec2( p_tilewidth, p_tilehight), vec2( l_x*p_tilewidth+l_x, l_y*p_tilehight+l_y));
}

void world::drawBackAndForeground( graphic *graphic) {
    // load tileset if not loadead
    if( p_tileset != NULL) {
        int l_camera_x = graphic->getCamera().x/p_tilewidth;
        int l_camera_y = graphic->getCamera().y/p_tilehight;

        int l_max_x = graphic->getCameraSize().x/p_tilewidth;
        int l_max_y = graphic->getCameraSize().y/p_tilehight;

        //
        for( int l_x = l_camera_x; l_x <= l_camera_x+l_max_x+1; l_x++) {
            for( int l_y = l_camera_y; l_y <= l_camera_y+l_max_y+1; l_y++) {
                drawTile( graphic, l_x, l_y, p_tilemap_background);
                if( p_tilemap_floorground != NULL)
                    drawTile( graphic, l_x, l_y, p_tilemap_floorground);
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
        for( int l_x = l_camera_x; l_x <= l_camera_x+l_max_x+1; l_x++) {
            for( int l_y = l_camera_y; l_y <= l_camera_y+l_max_y+1; l_y++) {
                drawTile( graphic, l_x, l_y, p_tilemap_overlap);
            }
        }
    }
}

void world::process( graphic *graphic) {
    fvec2 l_cam = graphic->getCamera();
    vec2 l_camSize = graphic->getCameraSize();

    // not over x
    if( l_cam.x < 0)
        l_cam.x = 0;

    // down
    if( l_cam.y > p_tilehight*p_map_hight - l_camSize.y)
        l_cam.y = p_tilehight*p_map_hight - l_camSize.y;

    // down
    if( l_cam.x > p_tilewidth*p_map_width - l_camSize.x)
        l_cam.x = p_tilewidth*p_map_width - l_camSize.x;

    // set camera x y
    graphic->setCamera( l_cam.tovec2());
    // drop old calc
    graphic->flipCamera();
}

void world::draw( graphic *graphic) {
    // dont draw if world not loaded or wrong loaded
    if( p_tilemap_foreground == NULL)
        return;

    if( (int)p_backgrounds.size() > 0) {
        if(p_backgrounds[0].picture == NULL)
            p_backgrounds[0].picture = graphic->loadImage( p_backgrounds[0].picture_file);
        else {
            world_background *l_background = &p_backgrounds[0];
            float l_factor = (float)l_background->picture->surface->h/ ((float)p_map_hight * (float)p_tilehight+p_tilehight);

            vec2 l_position = { graphic->getCamera().x*0.5, graphic->getCamera().y};

            while(l_position.x-graphic->getCamera().x+ l_background->picture->surface->w/l_factor< 0)
                l_position.x+=l_background->picture->surface->w/l_factor;

            // calc the new postion
            vec2 l_xvel = vec2( l_background->picture->surface->w/l_factor, 0);
            vec2 l_size = vec2( p_map_width * p_tilewidth+0.5, p_map_hight * p_tilehight+p_tilehight);

            // draw one left middle and right
            graphic->drawImage( l_background->picture, l_position, l_size, vec2( 0, 0), 0.0, 0, l_factor);
            graphic->drawImage( l_background->picture, l_position - l_xvel, l_size, vec2( 0, 0), 0.0, 0, l_factor);
            graphic->drawImage( l_background->picture, l_position + l_xvel, l_size, vec2( 0, 0), 0.0, 0, l_factor);
            graphic->drawImage( l_background->picture, l_position + l_xvel + l_xvel, l_size, vec2( 0, 0), 0.0, 0, l_factor);
        }
    }

    // draw Back and Foreground
    drawBackAndForeground( graphic);

    // draw Overground
    //drawOverground( graphic);
}

void world::loadImageFiles( graphic *graphic) {
    if( p_tileset == NULL) {
        p_tileset = graphic->loadImage( p_tilesetpath);
    }
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
        l_tile.type = findType( l_tile.id-1 );

        int x = l_amount, y = 0;

        while( x >= p_map_width) {
            x-= p_map_width;
            y++;
        }

        l_tile.pos = vec2( x, y);

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
