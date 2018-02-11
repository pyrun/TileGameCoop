#include "game.h"

game::game()
{
    // inertization
    initSDL();

    // config
    p_config = new config();

    // init graphic
    p_graphic = new graphic( p_config);

    // load font
    p_font = new font( p_graphic);

    // create the framerate counter
    p_framerate = new framenrate( );

    // input
    p_input = new input( p_config);

    // create player_list
    p_player = new player_handle();

    p_particles = new particle_list();

    // setup network
    p_network = new network( p_config);
    p_network->init();
    //p_network->startServer();

    // audio
    p_audio = new audio();
    p_audio->loadMusic( p_config->get( "folder", "audio", "music/"), atoi( p_config->get( "volume", "audio", "20").c_str()) );

    // game running
    p_game_running = true;
    p_config->set( "quit", "false", "game");
}

game::~game()
{
    // close network
    if( p_network)
        delete p_network;
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
    // free audio
    if( p_audio)
        delete p_audio;

    if( p_config)
        delete p_config;
}

void game::drawHUD() {
    // if now gamepads found display big at screen
    if( p_player->getPlayerAmount() <= 0) {
        p_font->drawMessage( p_graphic, "No gamepad found!", vec2( p_graphic->getCamera().tovec2().x, (int)p_graphic->getCameraSize().y/2+30+30 ), 1.2f, 255);
        p_font->drawMessage( p_graphic, "Please connect one", vec2( p_graphic->getCamera().tovec2().x, (int)p_graphic->getCameraSize().y/2+40+30 ), 1.2f, 255);
        p_font->drawMessage( p_graphic, "gamepad for playing", vec2( p_graphic->getCamera().tovec2().x, (int)p_graphic->getCameraSize().y/2+50+30 ), 1.2f, 255);
    } else if( p_player->getPlayerActive() == 0 ) {
        p_font->drawMessage( p_graphic, "No Player Active!", vec2( p_graphic->getCamera().tovec2().x, (int)p_graphic->getCameraSize().y/2+30+30 ), 1.2f, 255);
        p_font->drawMessage( p_graphic, "Press Start", vec2( p_graphic->getCamera().tovec2().x, (int)p_graphic->getCameraSize().y/2+40+30 ), 1.2f, 255);
    }

    // debug
    if( p_config->get( "debug", "game", "false") == "true") {
        // display info graphic
        char l_test[255];

        float l_wert = 1000.f/ ( (float)p_framerate->getFramerate() );

        sprintf( l_test, "%s%d %4.0f %dx%d", (p_framerate->getDelay() < 10)? "0":"", p_framerate->getDelay(), l_wert, (int)p_graphic->getCameraSize().x, (int)p_graphic->getCameraSize().y );
        p_font->drawMessage( p_graphic, l_test, p_graphic->getCamera().tovec2() + vec2( (int)p_graphic->getCameraSize().x, 10), 1.0f, 255,true);

        /*sprintf( l_test, "Nativ %dx%d", p_config.getDisplay().x, p_config.getDisplay().y);
        p_font->drawMessage( p_graphic, l_test, p_graphic->getCamera().tovec2() +vec2( (int)p_graphic->getCameraSize().x, 0), 1.0f, 255, true);
*/
        sprintf( l_test, "Gemepads: %d Player Champs: %d Figures: %d Player active: %d", p_player->getPlayerAmount(), p_level->getEntityList()->getAmountPlayerObject(), p_player->getAmountPlayerChamps(), p_player->getPlayerActive());
        p_font->drawMessage( p_graphic, l_test, p_graphic->getCamera().tovec2() +vec2( 0, (int)p_graphic->getCameraSize().y), 1.0f, 255, false, true);
    }
}

int game::process_graphic( std::string levelName) {
    int l_error;

    // load level
    p_level = new level( levelName.size()==0?p_config->get( "start_file", "game", "intro.tmx"):levelName.c_str(), "worlds/", p_graphic, p_player, p_config, p_audio);

    // set lua link
    lua_level_setLink( p_level);

    // at the moment we have no error
    l_error = 0;

    timer l_time;
    float l_delta = 0.0f;

    timer l_measure;

    // main loop
    while( p_game_running == true && p_input->handle( p_graphic->getWindow())) {
        l_delta = l_time.getTicks();

        l_measure.start();

        // PROCESS:
        // calc delta
        if( l_delta < 0.5)
            l_delta = 0.5;
        // start
        l_time.start();

        // flip camera
        p_graphic->flipCamera();

        // network
        if( p_level->getEntityList()) {
            p_network->process( p_level);
            if( p_network->isClient() && p_network->isStarted() && p_level->getEntityList()->getSync() == false)
                p_level->getEntityList()->setSync( true);
        }

        // react of player input
        p_player->handle( p_level->getEntityList(), p_level->getWorld(), p_input, p_graphic, p_config);

        // p_level process
        p_level->process( l_delta, p_config, p_graphic, p_player, p_particles, p_audio);

        // DRAW:
        // draw world
        p_level->getWorld()->draw( p_graphic);

        // draw entity
        p_level->getEntityList()->draw( p_graphic, p_particles, p_config, p_level->getWorld()->depthSort());

        // draw overground
        p_level->getWorld()->drawOverground( p_graphic);

        // draw info
        drawHUD();
        p_level->getEntityList()->drawHUD( p_graphic);

        // player draw
        p_player->draw( p_level->getEntityList(), p_font, p_graphic);

        // level draw
        p_level->draw( p_graphic);

        SDL_SetWindowTitle( p_graphic->getWindow(), patch::to_string( l_measure.getTicks()).c_str() );

        // draw particle
        p_particles->draw( p_graphic, p_font);

        // graphic clear/draw
        p_graphic->clear( l_delta);

        // now calc the delay for the framerate
        p_framerate->calc();

        // if someone will quit the program
        p_game_running = p_config->get( "quit", "game") == "false";

    }

    //delete test;

    return l_error;
}
