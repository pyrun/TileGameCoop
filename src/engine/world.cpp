#include "world.h"

#include <math.h>

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

float world::getCollisionX( fvec2 position, fvec2 change, fvec2 velocity, bool left) {
    int l_tempx;
    int l_x = ( position.x )/p_tilewidth;
    int l_y = ( position.y )/p_tilehight;
    tile *l_tile = NULL;

    // change in steps ann�hern
    for( float i = 0; i < fabs(change.x); i+= 0.1f) {
        l_x = ( position.x + i + p_tilewidth/2)/p_tilewidth;
        l_y = ( position.y )/p_tilehight;

        if( left)
            l_tempx = l_x - 1;
        else
            l_tempx = l_x;

        // collision tile
        l_tile = getTile( p_tilemap_foreground, l_tempx, l_y);
        if( l_tile != NULL && l_tile->id == 0) {
            l_tile = NULL;
            continue;
        }

        if( l_tile->type != NULL && l_tile->type->left && change.x > 0) {
            l_tile = NULL;
            continue;
        }
        break;
    }

    // zwischen rechnung
    float l_pos_change_x = position.x + change.x;
    float l_bottom = l_x*p_tilewidth;

    // ausrechnung der �nderung
    float l_result = l_pos_change_x-l_bottom;

    // keine tile gefunden
    if( l_tile == NULL)
        return MASSIV_TILE;

    // abfragen wo hitbox ist
    if( l_tile->id == 0)
        return MASSIV_TILE;

    // massiv -> rechnen
    if( fabs(l_result) <= fabs(change.x)+0.1) {
        // schauen ob die korrektur n�tig ist
        if( (l_result <= 0 && left) || l_result >= 0 && !left )
            return l_result;
    }

    // freier weg
    return MASSIV_TILE;
}

float world::getCollisionY( fvec2 position, fvec2 change, fvec2 velocity, bool up) {
    int l_tempy;
    int l_x = ( position.x )/p_tilewidth;
    int l_y = ( position.y )/p_tilehight;
    tile *l_tile = NULL;

    // change in steps ann�hern
    for( float i = 0; i < fabs(change.y); i+= 0.1f) {
        l_x = ( position.x )/p_tilewidth;
        l_y = ( position.y + i + p_tilehight/2)/p_tilehight;

        if( up)
            l_tempy = l_y - 1;
        else
            l_tempy = l_y;

        // collision tile
        l_tile = getTile( p_tilemap_foreground, l_x, l_tempy);
        if( l_tile != NULL && l_tile->id == 0) {
            l_tile = NULL;
            continue;
        }
        break;
    }

    // zwischen rechnung
    float l_pos_change_y = position.y + change.y;
    float l_bottom = l_y*p_tilehight;

    // ausrechnung der �nderung
    float l_result = l_pos_change_y-l_bottom;

    // keine tile gefunden
    if( l_tile == NULL)
        return MASSIV_TILE;

    // abfragen wo hitbox ist
    if( l_tile->id == 0)
        return MASSIV_TILE;

    // massiv -> rechnen
    if( fabs(l_result) <= fabs(change.y)+0.1) {
        // schauen ob die korrektur n�tig ist
        if( (l_result <= 0 && up) || l_result >= 0 && !up )
            return l_result;
    }

    // freier weg
    return MASSIV_TILE;
}

void world::loadTypes( std::string file) {
    XMLDocument l_file;
    XMLElement* l_tile;

    std::string l_xmlfile = file + ".xml";

    // if fille dont found
    if( file_exist( l_xmlfile) == false)
        return;

    // load the file
    XMLError l_result = l_file.LoadFile( l_xmlfile.c_str());

    l_tile = l_file.FirstChildElement( "animation");
    while( l_tile) {
        tiletype *l_type = new tiletype();
        XMLElement* l_animation_tile;

        if( l_tile->Attribute( "speed"))
            l_type->speed = atoi(l_tile->Attribute( "speed"));
        else
            l_type->speed = 0;


        l_type->down = 1;
        l_type->up = 1;
        l_type->right = 1;
        l_type->left = 1;

        if( l_tile->Attribute( "down"))
            l_type->down = atoi(l_tile->Attribute( "down"));
        if( l_tile->Attribute( "up"))
            l_type->up = atoi(l_tile->Attribute( "up"));
        if( l_tile->Attribute( "right"))
            l_type->right = atoi(l_tile->Attribute( "right"));
        if( l_tile->Attribute( "left"))
            l_type->left = atoi(l_tile->Attribute( "left"));


        l_animation_tile = l_tile->FirstChildElement("tile");
        while( l_animation_tile) {
            l_type->id.push_back( atoi(l_animation_tile->GetText()) );
            l_animation_tile = l_animation_tile->NextSiblingElement("tile");
        }

        p_tiletypes.push_back( *l_type);

        l_tile = l_tile->NextSiblingElement("animation");
    }

    //printf("%s %d types loaded\n", l_xmlfile.c_str(), (int)p_tiletypes.size());
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

        // background zuweisen
        if( l_name == "Background" )
            l_background = l_map_temp;
        if( l_name == "Foreground" )
            l_foreground = l_map_temp;
        if( l_name == "Overlap" )
            l_overlap = l_map_temp;

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

    // safe tileset path
    p_tilesetpath = ordner + l_tileset;
    p_tileset_width = l_tileset_width;
    p_tileset_height = l_tileset_height;

    p_tilewidth = l_tilewidth;
    p_tilehight = l_tilehight;

    //printf( "Tileset %s %d %d\n", p_tilesetpath.c_str(), p_tileset_width/p_tilewidth, l_tileset_height/l_tilehight);

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

tile *world::getTile( tile *tilemap, int x, int y) {
    tile *l_tile = NULL;

    // nicht �ber rand
    if( x >= p_map_width || y >= p_map_hight )
        return NULL;

    // keine negative werte
    if( x < 0 || y < 0)
        return NULL;


    return &tilemap[ y * p_map_width + x];;
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

    // Animation
    if( l_tile->type != NULL && l_tile->type->speed != 0)
            l_x = l_tile->type->id[ ((graphic->getFrame()/l_tile->type->speed) + getTypeIndex(l_tile->id, l_tile->type) )%l_tile->type->id.size()]+1;
    else
        l_x = l_tile->id;
    int l_y = 0;
    int l_factor = (p_tileset_width/(p_tilewidth+p_tileset_space));

    while( l_x > l_factor) {
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
        for( int l_x = l_camera_x; l_x <= l_camera_x+l_max_x+1; l_x++) {
            for( int l_y = l_camera_y; l_y <= l_camera_y+l_max_y+1; l_y++) {
                drawTile( graphic, l_x, l_y, p_tilemap_overlap);
            }
        }
    }
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
            float l_factor = (float)l_background->picture->surface->h/ ((float)p_map_hight * (float)p_tilehight);

            vec2 l_position = { graphic->getCamera().x*0.5, graphic->getCamera().y*0.5};

            int l_background_x = 0;
            while(l_position.x-graphic->getCamera().x+ l_background->picture->surface->w/l_factor< 0)
                l_position.x+=l_background->picture->surface->w/l_factor;

            graphic->drawImage( l_background->picture, l_position, vec2( p_map_width * p_tilewidth, p_map_hight * p_tilehight), vec2( 0, 0), 0.0, 0, l_factor);

            graphic->drawImage( l_background->picture, l_position + vec2( l_background->picture->surface->w/l_factor, 0), vec2( p_map_width * p_tilewidth, p_map_hight * p_tilehight), vec2( 0, 0), 0.0, 0, l_factor);

            graphic->drawImage( l_background->picture, l_position + vec2( l_background->picture->surface->w/l_factor, 0) + vec2( l_background->picture->surface->w/l_factor, 0), vec2( p_map_width * p_tilewidth, p_map_hight * p_tilehight), vec2( 0, 0), 0.0, 0, l_factor);

        }
    }

    // draw Back and Foreground
    drawBackAndForeground( graphic);

    // draw Overground
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
        l_tile.type = findType( l_tile.id-1 );

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
