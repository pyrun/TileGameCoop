#include "world.h"

using namespace tinyxml2;

#ifndef XMLCheckResult
	#define XMLCheckResult(a_eResult) if (a_eResult != XML_SUCCESS) { printf("Error: %i\n", a_eResult); return a_eResult; }
#endif

world::world(std::string file = "default.tmx")
{
    p_file = file;
    load( file);
}

world::~world()
{
    //dtor
}

bool world::load( std::string file = "") {
    if(file != "")
        p_file = file;

    XMLDocument l_file;

    //load the file
    XMLError l_result = l_file.LoadFile( file.c_str());

    // check the file
    XMLCheckResult(l_result);

    // map
    XMLElement* l_map = l_file.FirstChildElement( "map" );
    printf( "map %s %s\n", l_map->Attribute( "width" ), l_map->Attribute( "height" ));

    // tileset
    XMLElement* l_tileset = l_map->FirstChildElement( "tileset" );
    printf( "tileset %s %s\n", l_map->Attribute( "tilewidth" ), l_map->Attribute( "tileheight" ));

    // load the root node
    /*XMLElement* titleElement = l_file.FirstChildElement( "map" )->FirstChildElement( "layer" )->FirstChildElement( "data" );
    const char* title = titleElement->GetText();
    printf( "Name %s\n", layer->Attribute( "name" ) );*/

    return 1;
}

std::string world::loadParameter( XMLNode *root, std::string name) {
    XMLElement * l_element = root->FirstChildElement( name.c_str());
    if (l_element == nullptr) return "";

    const char *l_out;
    l_out = l_element->Value();

    return l_out;
}
