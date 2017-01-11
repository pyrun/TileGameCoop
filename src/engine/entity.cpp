#include "entity.h"

using namespace tinyxml2;

#ifndef XMLCheckResult
	#define XMLCheckResult(a_eResult) if (a_eResult != XML_SUCCESS) { printf("Error: %i\n", a_eResult); return a_eResult; }
#endif

action* entitytype::getAction( std::string name) {
    for( int i = 0; i < (int)p_actions.size(); i++) {
        if( p_actions[i].name == name)
            return &p_actions[i];
    }

    return NULL;
}

entity::entity( int id)
{
    // set id
    p_id = id;
}

entity::~entity()
{
    //dtor
}

void entity::draw( graphic *graphic) {
    int l_frame;

    action *l_action = p_type->getAction( this->p_action);
    if( l_action == NULL) { // falls animaton fehlt zurück zu idle
        p_action = ACTION_IDLE;
        printf("dont found action, jump to idle\n");
        return;
    }

    image *l_image = l_action->imagefile;



    if( l_action->frame != 0)
        l_frame = p_type->getWidth()*( ((int)(graphic->getFrame()/l_action->speed) ) %l_action->frame);
    else
        l_frame = 0;

    graphic->drawImage( l_image, p_pos, vec2( p_type->getWidth(),p_type->getHeight()), vec2( l_frame, 0));
}

entitylist::entitylist()
{
    // start at 0
    p_id = 0;
}

entitylist::~entitylist()
{
    //dtor
}

int entitylist::create( entitytype *type, vec2 pos) {
    entity* obj;

    // check data
    if( type == NULL)
        return -1;

    printf( "creating \"%s\" with id %d\n", type->getName().c_str(), p_id);

    // create object
    obj = new entity( p_id);
    // set data
    obj->setAction( ACTION_IDLE);
    obj->setType( type);
    obj->setPos( pos);

    // incress next id
    p_id++;

    // add to vector
    p_entitys.push_back( *obj);

    return (p_id-1);
}

void entitylist::draw(graphic *graphic) {
    for(int i = 0; i < (int)p_entitys.size(); i++)
        p_entitys[i].draw( graphic);
}

bool entitylist::loadType( std::string folder, graphic *graphic) {
    XMLDocument l_file;

    p_folder = folder;

    std::string l_pathfile = folder + (char*)ENTITY_FILE;

    int l_width;
    int l_height;

    std::string l_name;
    std::vector<action> *l_actions = new std::vector<action>();

    // load the file
    XMLError l_result = l_file.LoadFile( l_pathfile.c_str());

    // check the file
    XMLCheckResult(l_result);

    //
    XMLElement* l_object = l_file.FirstChildElement( "object" );
    if( !l_object)
        return false;

    l_name = l_object->Attribute( "name" );
    l_width = atoi(l_object->Attribute( "width" ));
    l_height = atoi(l_object->Attribute( "height" ));

    // read all actions
    XMLElement* l_xml_action = l_file.FirstChildElement( "action" );
    /*if( !l_xml_action)
        return -1;*/
    entitytype *l_type = new entitytype();
    std::string l_action_name;
    std::string l_action_file;
    int l_action_frame;
    int l_action_speed;

    bool l_idle = false;

    while( l_xml_action) {
        l_action_file = l_xml_action->GetText();
        l_action_name = l_xml_action->Attribute( "name" );
        l_action_frame = atoi( l_xml_action->Attribute( "frame" ));
        l_action_speed = atoi( l_xml_action->Attribute( "speed" ));

        // check data
        if( l_action_speed == 0)
            l_action_speed = 1; // verry fast animation

        if( l_action_name == ACTION_IDLE)
            l_idle = true;

        // push back
        l_type->addAction( l_action_name, l_action_file, l_action_frame, l_action_speed, graphic->loadImage( folder + l_action_file));

        l_xml_action = l_xml_action->NextSiblingElement("action");
    }

    if( !l_idle) {
        printf("entitylist::loadType type %s has no idle action!\n", l_name.c_str());
        return false;
    }

    l_type->setWidth(l_width);
    l_type->setHeight(l_height);
    l_type->setName( l_name);

    p_entity_types.push_back( *l_type);

    return true;
}

entitytype* entitylist::getType( std::string name) {
    for( int i = 0; i < (int)p_entity_types.size(); i++)
        if( p_entity_types[i].getName() == name)
            return &p_entity_types[i];
    return NULL;
}

entity* entitylist::getEntity( int id) {
    for( int i = 0; i < (int)p_entitys.size(); i++)
        if( p_entitys[i].getId() == id)
            return &p_entitys[i];
    return NULL;
}
