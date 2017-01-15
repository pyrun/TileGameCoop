#include "config.h"

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
    setDisplaySizeFullscreen( vec2( loadParameter( l_root, "DisplayFullscreen_width"), loadParameter( l_root, "DisplayFullscreen_height")) );
    setDisplayMode( loadParameter( l_root, "Display_mode"));

    setDisplayMaximized( loadParameter( l_root, "Display_maximized"));

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
    saveParameter( &l_config, l_root, "DisplayFullscreen_width", getDisplayFullscreen().x);
    saveParameter( &l_config, l_root, "DisplayFullscreen_height", getDisplayFullscreen().y);
    saveParameter( &l_config, l_root, "Display_mode", getDisplayMode());

    saveParameter( &l_config, l_root, "Display_maximized", getDisplayMaximized());

    // save
    l_config.SaveFile( CONFIG_FILE);
}

void config::saveParameter( XMLDocument *config, XMLNode *root, std::string name, int data) {
    XMLElement * l_element = config->NewElement( name.c_str());
    l_element->SetText( data);
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
