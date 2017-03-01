#include "input.h"

#include <stdio.h>

input_map::input_map() {
    x = 0;
    y = 0;
    run = false;
    jump = false;
    attack = false;
    special = false;
    select = false;
    start = false;
    left = false;
    right = false;
}

input::input( config *config)
{
    // add more mapping for gamecontroller
    SDL_GameControllerAddMappingsFromFile( config->getControllerMappingsFile().c_str());

    // add all exist gamepads
    /*for (int i = 0; i < SDL_NumJoysticks(); i++) {
        if (SDL_IsGameController(i))
            p_device.push_back( i);
    }*/

    // link config
    p_config = config;
}

input::~input()
{
    //dtor
}

bool input::handle( SDL_Window* window) {
    // flag
    p_resize = false;

    while( SDL_PollEvent(&p_event) ) {
        // handle gamepads
        if(p_event.type == SDL_CONTROLLERDEVICEADDED) {
            printf( "input::handle add new controller %d\n", p_event.cdevice.which);
            p_device.push_back( p_event.cdevice.which);
        }
        if(p_event.type == SDL_CONTROLLERDEVICEREMOVED)
            p_deviceDestroyed.push_back( p_event.cdevice.which);

        // get the type of event
        if (p_event.type == SDL_WINDOWEVENT) {
            // chose what to do
            switch (p_event.window.event) {
                // resize of the windows event
                case SDL_WINDOWEVENT_RESIZED:
                    p_resize = true;
                    p_display_width = p_event.window.data1;
                    p_display_height = p_event.window.data2;

                    // set display
                    p_config->setDisplay( p_display_width, p_display_height);
                    p_config->setDisplayChangeMode();
                break;
            }
        }

        // quit event
        if(p_event.type == SDL_QUIT)
            return false;

    }

    return true;
}
