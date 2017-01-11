#include "game.h"

game::game()
{
    // inertization
    initSDL();

    // init graphic
    p_graphic = new graphic( &p_config);

    // create the framerate counter
    p_framerate = new framenrate();

    // create player_list
    p_player = new player_handle(&p_config);

    // input
    p_input = new input( &p_config);

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
    float wert = 1000/ (16-(float)p_framerate->getDelay() );
    sprintf( test, "%s%d %4.0f %dx%d", (p_framerate->getDelay() < 10)? "0":"", p_framerate->getDelay(), wert,NATIV_W, NATIV_H );
    p_font->drawMessage( p_graphic, test, vec2( NATIV_W-140, 10));
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
        std::string file = l_path + l_entry->d_name + "/";

        // load folder
        p_entity->loadType( file, p_graphic);
    }
    closedir(l_dir);
}

int game::process() {
    l_timer.start();
    //p_graphic->moveCamera( { 1, 0});

    //if( p_graphic->getCamera().y > 500)
    //    p_graphic->setCamera( { 20, 0});

    return l_timer.getTicks();
}

int game::process_graphic() {
    int l_error;


    p_world = new world( "1-1.tmx", "worlds/");

    // at the moment we have no error
    l_error = 0;

    p_entity->create( p_entity->getType("coin"), vec2( 200, 100));
    int riven = p_entity->create( p_entity->getType("riven"), vec2( 100, 100));

    p_entity->create( p_entity->getType("coin2"), vec2( 200, 200));


    p_entity->getEntity( riven)->setAction( "run");

    // main loop
    while( p_game_running == true && p_input->handle( p_graphic->getWindow())) {
        // start measurement point
        p_framerate->begin();

        // flip camera
        p_graphic->flipCamera();

        // react of player input
        p_player->handle();

        // process
        process();

        // draw world
        p_world->draw( p_graphic);

        // draw entity
        p_entity->draw( p_graphic);

        // draw info
        drawHUD();

        // graphic clear/draw
        p_graphic->clear();

        // now calc the delay for the framerate
        p_framerate->calc();
    }

    return l_error;
}
