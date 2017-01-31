#include "game.h"

game::game()
{
    // inertization
    initSDL();

    // init graphic
    p_graphic = new graphic( &p_config);

    // create the framerate counter
    p_framerate = new framenrate( );

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
        std::string file = l_path + l_entry->d_name + "/";

        // load folder
        p_entity->loadType( file, p_graphic);
    }
    closedir(l_dir);
}

int game::process() {
    p_timer.start();
    //p_graphic->moveCamera( { 1, 0});

    //if( p_graphic->getCamera().y > 500)
        p_graphic->setCamera( { 0, 0});

    return p_timer.getTicks();
}

int game::process_graphic() {
    int l_error;

    p_world = new world( "1-1.tmx", "worlds/");
    p_entity->createFromWorldFile( p_world->getFileName());

    // at the moment we have no error
    l_error = 0;

    //p_entity->create( p_entity->getType("coin"), vec2( 40, 80));

    /*int riven = p_entity->create( p_entity->getType("knight"), vec2( 10, 100));

    p_entity->getEntity( riven)->setAction( "jump");*/


    //delta time start
    p_deltaTime.start();

    printf( "%d\n", SDL_CONTROLLER_BUTTON_X );

    // main loop
    while( p_game_running == true && p_input->handle( p_graphic->getWindow())) {
        // start measurement point
        //p_framerate->begin();

        //p_entity->create( p_entity->getType("knight"), vec2( 10, 100));

        // flip camera
        p_graphic->flipCamera();

        // react of player input
        p_player->handle( p_entity);

        // process
        process();

        // calc entity
        p_entity->process( p_world, p_deltaTime.getTicks());

        //delta time reset to next frame
        p_deltaTime.start();

        // draw world
        p_world->draw( p_graphic);

        /*if(p_graphic->getFrame()%100 == 0) {
            p_entity->getEntity( riven)->setVelocity( fvec2( 0, -0.285f) );
            p_entity->getEntity( riven)->setAction( "jump");
        } else
            if( p_entity->getEntity( riven)->getVelocity().y > 0)
                p_entity->getEntity( riven)->setAction( "walk");*/

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
