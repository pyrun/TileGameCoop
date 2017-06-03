#include "config.h"


/** LUA FUNCTION */

config *lua_config = NULL;

static int lua_setconfig( lua_State *state) {
    std::string l_name;
    bool l_set;

    if( !lua_isstring( state, 1) || !lua_isboolean( state, 2) ) {
        printf( "lua_setconfig call wrong argument\n");
        return 0;
    }

    l_name = lua_tostring( state, 1);
    l_set = lua_toboolean( state, 2);

    if( l_name == "Fullscreen") {
        lua_config->setDisplayMode( l_set);
        lua_config->setDisplayChangeMode();
        lua_config->setDisplay( 604, 400); // just a magic resolution
    }
    if( l_name == "Debug")
        lua_config->setDebug( l_set);

    return 0;
}

static int lua_getconfig( lua_State *state) {
    std::string l_name;
    bool l_set = false;

    if( !lua_isstring( state, 1) ) {
        printf( "lua_getconfig call wrong argument\n");
        return 0;
    }

    l_name = lua_tostring( state, 1);

    if( l_name == "Fullscreen")
        l_set = lua_config->getDisplayMode();
    if( l_name == "Debug")
        l_set = lua_config->getDebug();

    lua_pushboolean( state, l_set);

    return 1;
}

void lua_config_install( lua_State *state) {
    // add all entity function ..
    lua_pushcfunction( state, lua_setconfig);
    lua_setglobal( state, "setconfig");

    lua_pushcfunction( state, lua_getconfig);
    lua_setglobal( state, "getconfig");
}

void lua_config_setLink( config* config) {
    // set list
    lua_config = config;
}


using namespace tinyxml2;

#ifndef XMLCheckResult
	#define XMLCheckResult(a_eResult) if (a_eResult != XML_SUCCESS) { printf("Error: %i\n", a_eResult); return a_eResult; }
#endif

config::config()
{
#ifdef DEBUG_CONFIG
    printf("start config...");
#endif // DEBUG_CONFIG

    // load config
    load();

#ifdef DEBUG_CONFIG
    printf("fine\n");
#endif // DEBUG_CONFIG
    p_display_changemode = false;
}

config::~config()
{
#ifdef DEBUG_CONFIG
    printf("close config...");
#endif // DEBUG_CONFIG

    // load config
    save();

#ifdef DEBUG_CONFIG
    printf("finish\n");
#endif // DEBUG_CONFIG
}

int config::load() {
    XMLDocument l_config;

    //load the file
    XMLError l_result = l_config.LoadFile(CONFIG_FILE);

    // check the file
    XMLCheckResult(l_result);

    // load the root node
    XMLNode * l_root = l_config.FirstChild();
    if (l_root == nullptr) return XML_ERROR_FILE_READ_ERROR;

    // load data
    setDisplay( loadParameter( l_root, "Display_width"), loadParameter( l_root, "Display_height"));
    setDisplayMode( loadParameter( l_root, "Display_mode"));

    setDisplayMaximized( loadParameter( l_root, "Display_maximized"));

    setDisplayResolutionFile( loadParameterString( l_root, "Display_resolution_file"));

    // load input
    setInputPadButton( loadParameter( l_root, "Input_run"),
                       loadParameter( l_root, "Input_jump"),
                       loadParameter( l_root, "Input_attack"),
                       loadParameter( l_root, "Input_special"),
                       loadParameter( l_root, "Input_select"),
                       loadParameter( l_root, "Input_start"),
                       loadParameter( l_root, "Input_left"),
                       loadParameter( l_root, "Input_right"));

    // input axis
    setInputPadAxis(    loadParameter( l_root, "Input_x"),
                        loadParameter( l_root, "Input_y"));

    setControllerMappingsFile( loadParameterString( l_root, "ControllerMappingsFile"));

    setDebug( loadParameter( l_root, "Debug"));

    // return a success of loading
    return XML_SUCCESS;
}

void config::save() {
    XMLDocument l_config;

    // create root node
    XMLNode * l_root = l_config.NewElement("Root");

    // insert the root node
    l_config.InsertFirstChild(l_root);

    // input
    saveParameter( &l_config, l_root, "Input_run", getInputPadButton_run() );
    saveParameter( &l_config, l_root, "Input_jump", getInputPadButton_jump() );
    saveParameter( &l_config, l_root, "Input_attack", getInputPadButton_attack() );
    saveParameter( &l_config, l_root, "Input_special", getInputPadButton_special() );
    saveParameter( &l_config, l_root, "Input_select", getInputPadButton_select() );
    saveParameter( &l_config, l_root, "Input_start", getInputPadButton_start() );
    saveParameter( &l_config, l_root, "Input_left", getInputPadButton_left() );
    saveParameter( &l_config, l_root, "Input_right", getInputPadButton_right() );

    // input axis
    saveParameter( &l_config, l_root, "Input_x", getInputPadAxisX() );
    saveParameter( &l_config, l_root, "Input_y", getInputPadAxisY() );

    // save parameter
    saveParameter( &l_config, l_root, "Display_width", getDisplay().x);
    saveParameter( &l_config, l_root, "Display_height", getDisplay().y);
    saveParameter( &l_config, l_root, "Display_mode", getDisplayMode());

    saveParameter( &l_config, l_root, "Display_maximized", getDisplayMaximized());
    saveParameter( &l_config, l_root, "Display_resolution_file", getDisplayResolutionFile());

    saveParameter( &l_config, l_root, "ControllerMappingsFile", getControllerMappingsFile());

    saveParameter( &l_config, l_root, "Debug", getDebug());

    // save
    l_config.SaveFile( CONFIG_FILE);
}

void config::saveParameter( XMLDocument *config, XMLNode *root, std::string name, int data) {
    XMLElement * l_element = config->NewElement( name.c_str());
    l_element->SetText( data);
    root->InsertEndChild(l_element);
}

void config::saveParameter( XMLDocument *config, XMLNode *root, std::string name, std::string data) {
    XMLElement * l_element = config->NewElement( name.c_str());
    l_element->SetText( data.c_str());
    root->InsertEndChild(l_element);
}

int config::loadParameter( XMLNode *root, std::string name) {
    XMLElement * l_element = root->FirstChildElement( name.c_str());
    if (l_element == nullptr) return XML_ERROR_PARSING_ELEMENT;

    int l_out;
    XMLError l_result = l_element->QueryIntText(&l_out);
    XMLCheckResult(l_result);

    return l_out;
}

std::string config::loadParameterString( XMLNode *root, std::string name) {
    XMLElement * l_element = root->FirstChildElement( name.c_str());
    if (l_element == nullptr) return "";

    return l_element->GetText();
}
