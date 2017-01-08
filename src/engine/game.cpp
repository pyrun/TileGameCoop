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

    // game running
    p_game_running = true;
}

game::~game()
{
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

int game::process() {

    //p_graphic->moveCamera( { 1, 0});

    //if( p_graphic->getCamera().y > 500)
    //    p_graphic->setCamera( { 20, 0});
}

int game::process_graphic() {
    int l_error;

    p_world = new world( "1-1.tmx", "worlds/");

    // at the moment we have no error
    l_error = 0;

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

//        p_graphic->drawImage( img, vec2(0,0), vec2( 50, 50), vec2( 0, 0), 0, 0);
//        p_graphic->drawImage( img, vec2(50,00), vec2( 50, 50), vec2( 50, 00), 0, 0);

        p_world->draw( p_graphic);

        //p_font->drawLetter( p_graphic, 'T');
        char test[255];
        int wert = 5;
        sprintf( test, "%s%d %dx%d", (p_framerate->getDelay() < 10)? "0":"", p_framerate->getDelay(),NATIV_W, NATIV_H );
        p_font->drawMessage( p_graphic, test, vec2( NATIV_W-140, 10));

        // graphic clear/draw
        p_graphic->clear();

        // now calc the delay for the framerate
        p_framerate->calc();
    }

    return l_error;
}
