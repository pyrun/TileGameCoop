#include "game.h"

Uint32 callbackCalc(Uint32 interval, void *param) {

    game *l_game = (game *)param;

    //p_graphic->moveCemara( { 1, 0});
    l_game->process();

    return interval;
}

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

    //
    //p_graphic->moveCamera( { 1, 0});

    //if( p_graphic->getCamera().y > 500)
    //    p_graphic->setCamera( { 0, -500});
}

int game::process_graphic() {
    int l_error;

    p_world = new world( "1-1.tmx", "worlds/");

    p_timer = SDL_AddTimer( 10, callbackCalc, this);

    // at the moment we have no error
    l_error = 0;

    // main loop
    while( p_game_running == true && p_input->handle( p_graphic->getWindow())) {
        SDL_Delay( 1);
        // start measurement point
       // p_framerate->begin();

        // react of player input
        p_player->handle();

        // now calc the delay for the framerate
        //p_framerate->calc();

//        p_graphic->drawImage( img, vec2(0,0), vec2( 50, 50), vec2( 0, 0), 0, 0);
//        p_graphic->drawImage( img, vec2(50,00), vec2( 50, 50), vec2( 50, 00), 0, 0);
        //p_world->draw( p_graphic);


        // graphic clear/draw
        p_graphic->clear();
    }

    return l_error;
}
