#include "game.h"

game::game()
{
    // inertization
    initSDL();

    // init graphic
    p_graphic = new graphic( &p_config);

    // create the framerate counter
    p_framerate = new framenrate( );

        // input
    p_input = new input( &p_config);

    // create player_list
    p_player = new player_handle();

    // no wolrd load
    p_world = NULL;

    // load font
    p_font = new font( p_graphic);

    // create entity list
    p_entity = new entitylist();

    // load types
    loadTypes();

    // game running
    p_game_running = true;
    p_config.setQuit( false);
}

game::~game()
{
    // delete entity
    if( p_entity)
        delete p_entity;

    // delte font
    if( p_font)
        delete p_font;

    // delete world if a there
    if( p_world)
        delete p_world;
    // free input
    if( p_input)
        delete p_input;
    // free player
    if( p_player)
        delete p_player;
    // free graphic
    if( p_graphic)
        delete p_graphic;
    // delete framecounter
    if( p_framerate)
        delete p_framerate;
}

void game::drawHUD() {
    char test[255];

    float wert = 1000.f/ ( (float)p_framerate->getFramerate() );

    sprintf( test, "%s%d %4.0f %dx%d", (p_framerate->getDelay() < 10)? "0":"", p_framerate->getDelay(), wert, (int)p_graphic->getCameraSize().x, (int)p_graphic->getCameraSize().y );
    p_font->drawMessage( p_graphic, test, vec2( (int)p_graphic->getCameraSize().x, 10), 1.0f,true);

    p_font->drawMessage( p_graphic, "Go home your drunk", vec2( 0, 0));

    sprintf( test, "Nativ %dx%d", p_config.getDisplay().x, p_config.getDisplay().y);
    p_font->drawMessage( p_graphic, test, vec2( (int)p_graphic->getCameraSize().x, 30), 1.0f, true);

    sprintf( test, "%d Player %d Figuren %d Player aktiv", p_player->getPlayerAmount(), p_entity->getAmountPlayerObject(), p_player->player_getPlayerActive());
    p_font->drawMessage( p_graphic, test, vec2( 0, (int)p_graphic->getCameraSize().y), 1.0f, false, true);
}

void game::loadTypes() {
    DIR *l_dir;

    struct dirent *l_entry;

    std::string l_path = "creature/";

    l_dir = opendir(l_path.c_str());
    if ( l_dir == NULL) {  /* error opening the directory? */
        printf("game::loadTypes cant load types, dir not found\n");
    }

    while ((l_entry = readdir(l_dir)) != NULL) {
        std::string l_file = l_path + l_entry->d_name + "/";

        // load folder
        p_entity->loadType( l_file, p_graphic);

    }
    closedir(l_dir);
}

int game::process() {
    p_timer.start();
    //p_graphic->moveCamera( { 1, 0});

    //if( p_graphic->getCamera().y > 500)
    //p_graphic->setCamera( { 0, 0});

    return p_timer.getTicks();
}

int game::process_graphic() {
    int l_error;

    p_world = new world( "cloud_1.tmx", "worlds/");
    p_entity->createFromWorldFile( p_world->getFileName());
    p_world->loadImageFiles( p_graphic);

    // at the moment we have no error
    l_error = 0;

    timer l_time;
    float l_delta = 0.0f;

    // main loop
    while( p_game_running == true && p_input->handle( p_graphic->getWindow())) {
        l_delta = l_time.getTicks();
        if( l_delta < 0.5)
            l_delta = 0.5;
        // start
        l_time.start();

        // flip camera
        p_graphic->flipCamera();

        // react of player input
        p_player->handle( p_entity, p_input, p_graphic, &p_config);

        p_world->process( p_graphic);

        // process
        process();

        p_entity->process( p_world, l_delta);

        // draw world
        p_world->draw( p_graphic);

        // draw entity
        p_entity->draw( p_graphic);

        p_world->drawOverground( p_graphic);

        // draw info
        drawHUD();

        // graphic clear/draw
        p_graphic->clear( l_delta);

        // now calc the delay for the framerate
        p_framerate->calc();

        // if someome will quit the program
        p_game_running = !(p_config.getQuit());

    }

    return l_error;
}
