#include "game.h"

game::game()
{
    // inertization
    initSDL();

    // init graphic
    p_graphic = new graphic( &p_config);

    // load font
    p_font = new font( p_graphic);

    // create the framerate counter
    p_framerate = new framenrate( );

    // input
    p_input = new input( &p_config);

    // create player_list
    p_player = new player_handle();

    p_particles = new particle_list();

    // game running
    p_game_running = true;
    p_config.setQuit( false);
}

game::~game()
{
    // delte font
    if( p_font)
        delete p_font;
    // delete all levels
    if( p_level)
        delete p_level;
    // free input
    if( p_input)
        delete p_input;
    // free player
    if( p_player)
        delete p_player;
    // delete the particle system
    if( p_particles)
        delete p_particles;
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
    p_font->drawMessage( p_graphic, test, p_graphic->getCamera().tovec2() + vec2( (int)p_graphic->getCameraSize().x, 10), 1.0f, 255,true);

    //p_font->drawMessage( p_graphic, "Go home your drunk", vec2( 0, 0));

    sprintf( test, "Nativ %dx%d", p_config.getDisplay().x, p_config.getDisplay().y);
    p_font->drawMessage( p_graphic, test, p_graphic->getCamera().tovec2() +vec2( (int)p_graphic->getCameraSize().x, 0), 1.0f, 255, true);

    sprintf( test, "%d Player %d PlayerChamps %d Figuren %d Player aktiv", p_player->getPlayerAmount(), p_level->getEntityList()->getAmountPlayerObject(), p_player->getAmountPlayerChamps(), p_player->getPlayerActive());
    p_font->drawMessage( p_graphic, test, p_graphic->getCamera().tovec2() +vec2( 0, (int)p_graphic->getCameraSize().y), 1.0f, 255, false, true);
}

int game::process_graphic( std::string levelName) {
    int l_error;

    //levelName = "world_0.tmx";

    p_level = new level( levelName.size()==0?"overworld.tmx":levelName.c_str(), "worlds/", p_graphic, p_player);

    p_particles->createParticel( par_text, fvec2( 100,100), fvec2(), 10*100, "test");

    // at the moment we have no error
    l_error = 0;

    timer l_time;
    float l_delta = 0.0f;

    // main loop
    while( p_game_running == true && p_input->handle( p_graphic->getWindow())) {
        l_delta = l_time.getTicks();

        // PROCESS:
        // calc delta
        if( l_delta < 0.5)
            l_delta = 0.5;
        // start
        l_time.start();

        // flip camera
        p_graphic->flipCamera();

        // react of player input
        p_player->handle( p_level->getEntityList(), p_input, p_graphic, &p_config);

        p_level->process( l_delta, &p_config, p_graphic, p_player, p_particles);

        // DRAW:
        // draw world
        p_level->getWorld()->draw( p_graphic);

        // draw entity
        p_level->getEntityList()->draw( p_graphic, p_particles, &p_config);

        // draw particle
        p_particles->draw( p_graphic, p_font);

        // draw overground
        p_level->getWorld()->drawOverground( p_graphic);

        // draw info
        drawHUD();

        // player draw
        p_player->draw( p_level->getEntityList(), p_font, p_graphic);

        // level draw
        p_level->draw( p_graphic);

        // graphic clear/draw
        p_graphic->clear( l_delta);

        // now calc the delay for the framerate
        p_framerate->calc();

        // if someone will quit the program
        p_game_running = !(p_config.getQuit());

    }

    return l_error;
}
