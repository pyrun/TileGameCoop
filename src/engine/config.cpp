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

    if( l_name == "fullscreen") {
        lua_config->setDisplayFullscreen( l_set);
        lua_config->setDisplayChangeMode();
        lua_config->setDisplay( 604, 400); // just a magic resolution
    }
    if( l_name == "debug")
        lua_config->setDebug( l_set);
    if( l_name == "exit" || l_name == "quit")
        lua_config->setQuit( true);


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

    if( l_name == "fullscreen")
        l_set = lua_config->isDisplayFullscreen();
    if( l_name == "debug")
        l_set = lua_config->getDebug();
    if( l_name == "quit")
        l_set = lua_config->getQuit();

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
    printf("config::config loading ...");
#endif // DEBUG_CONFIG

    // load config
    load();

#ifdef DEBUG_CONFIG
    printf("finish\n");
#endif // DEBUG_CONFIG
    p_display_changemode = false;
}

config::~config()
{
#ifdef DEBUG_CONFIG
    printf("config::~config save...");
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

    // get input node
    XMLElement* l_xml_input = l_root->FirstChildElement( "input" );
    if( l_xml_input) {
        // load input
        setInputPadButton( loadParameter( l_xml_input, "run"),
                           loadParameter( l_xml_input, "jump"),
                           loadParameter( l_xml_input, "attack"),
                           loadParameter( l_xml_input, "special"),
                           loadParameter( l_xml_input, "select"),
                           loadParameter( l_xml_input, "start"),
                           loadParameter( l_xml_input, "left"),
                           loadParameter( l_xml_input, "right"));

        // input axis
        setInputPadAxis(    loadParameter( l_xml_input, "x_axis"),
                            loadParameter( l_xml_input, "y_axis"));
    }

    // get display node
    XMLElement* l_xml_display = l_root->FirstChildElement( "display" );
    if( l_xml_display) {
        // load the resolution
        setDisplay( loadParameter( l_xml_display, "width"), loadParameter( l_xml_display, "height"));

        // get display flags
        setDisplayFullscreen( loadParameter( l_xml_display, "isFullScreen"));
        setDisplayMaximized( loadParameter( l_xml_display, "isMaximized"));

        // get display resolution file
        setDisplayResolutionFile( loadParameterString( l_xml_display, "resolution_file"));
    }

    // get display node
    XMLElement* l_xml_game = l_root->FirstChildElement( "game" );
    if( l_xml_game) {
        // get controller mapping file
        setControllerMappingsFile( loadParameterString( l_xml_game, "controllerMappingsFile"));

        // load the rest
        setDebug( loadParameter( l_xml_game, "debug"));
        setStartfile( loadParameterString( l_xml_game, "start_file"));
        setForceJoin( loadParameter( l_xml_game, "force_join"));
    }

    // get audio node
    XMLElement* l_xml_audio = l_root->FirstChildElement( "audio" );
    if( l_xml_audio) {
        setMusicFolder( loadParameterString( l_xml_audio, "folder"));
        setMusicVolume( loadParameter( l_xml_audio, "volume"));
    }

    // return a success of loading
    return XML_SUCCESS;
}

void config::save() {
    XMLDocument l_config;

    // create root node
    XMLNode * l_root = l_config.NewElement("config");

    // insert the root node
    l_config.InsertFirstChild(l_root);

    // creating input node
    XMLElement *l_xmlInput = l_config.NewElement( "input");
    // look if node corrupt
    if( l_xmlInput) {
        // save the parameters
        saveParameter( &l_config, l_xmlInput, "run", getInputPadButton_run() );
        saveParameter( &l_config, l_xmlInput, "jump", getInputPadButton_jump() );
        saveParameter( &l_config, l_xmlInput, "attack", getInputPadButton_attack() );
        saveParameter( &l_config, l_xmlInput, "special", getInputPadButton_special() );
        saveParameter( &l_config, l_xmlInput, "select", getInputPadButton_select() );
        saveParameter( &l_config, l_xmlInput, "start", getInputPadButton_start() );
        saveParameter( &l_config, l_xmlInput, "left", getInputPadButton_left() );
        saveParameter( &l_config, l_xmlInput, "right", getInputPadButton_right() );
        // input axis
        saveParameter( &l_config, l_xmlInput, "x_axis", getInputPadAxisX() );
        saveParameter( &l_config, l_xmlInput, "y_axis", getInputPadAxisY() );

        // link to the root node
        l_root->LinkEndChild( l_xmlInput);
    }

    // creating display node
    XMLElement *l_xmlDisplay = l_config.NewElement( "display");
    // look if node corrupt
    if( l_xmlDisplay) {
        // save parameter
        saveParameter( &l_config, l_xmlDisplay, "width", getDisplay().x);
        saveParameter( &l_config, l_xmlDisplay, "height", getDisplay().y);
        saveParameter( &l_config, l_xmlDisplay, "isFullScreen", isDisplayFullscreen());

        saveParameter( &l_config, l_xmlDisplay, "isMaximized", isDisplayMaximized());
        saveParameter( &l_config, l_xmlDisplay, "resolution_file", getDisplayResolutionFile());

        // link to the root node
        l_root->LinkEndChild( l_xmlDisplay);
    }

    // creating game node
    XMLElement *l_xmlGame = l_config.NewElement( "game");
    // look if node corrupt
    if( l_xmlGame) {
        // save parameter
        saveParameter( &l_config, l_xmlGame, "debug", getDebug());
        saveParameter( &l_config, l_xmlGame, "start_file", getStartfile());
        saveParameter( &l_config, l_xmlGame, "force_join", getForceJoin());
        // game pad driver file
        saveParameter( &l_config, l_xmlGame, "controllerMappingsFile", getControllerMappingsFile());

        // link to the root node
        l_root->LinkEndChild( l_xmlGame);
    }

    // creating audio node
    XMLElement *l_xmlAudio = l_config.NewElement( "audio");
    // look if node corrupt
    if( l_xmlAudio) {
        // save parameter
        saveParameter( &l_config, l_xmlAudio, "folder", getMusicFolder());
        saveParameter( &l_config, l_xmlAudio, "volume", getMusicVolume());
        // link to the root node
        l_root->LinkEndChild( l_xmlAudio);
    }

    // save
    l_config.SaveFile( CONFIG_FILE);
}

void config::saveParameter( XMLDocument *config, XMLNode *root, std::string name, int data) {
    XMLElement * l_element = config->NewElement( name.c_str());
    l_element->SetText( data);
    root->LinkEndChild( l_element);
}

void config::saveParameter( XMLDocument *config, XMLNode *root, std::string name, std::string data) {
    XMLElement * l_element = config->NewElement( name.c_str());
    l_element->SetText( data.c_str());
    root->LinkEndChild(l_element);
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
