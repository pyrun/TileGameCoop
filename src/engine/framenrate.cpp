#include "framenrate.h"

#include <stdio.h>

framenrate::framenrate( int framerate)
{
    // set values
    p_frame = 0;

    // delay
    p_ms = 0;

    // set framerate what we want
    p_framerate = framerate;

    // start the counter
    p_fps.start();
}

framenrate::~framenrate()
{
    //dtor
}

void framenrate::begin() {
    // start the frame timer

}

void framenrate::calc() {
    // Increment the frame counter
    p_frame++;

    // ms
    p_ms = p_fps.getTicks();
    p_ms_list.push_back( p_ms);
    if( (int)p_ms_list.size() > 20)
        p_ms_list.erase( p_ms_list.begin() );
    if( p_ms < p_framerate ) {
        p_fps.start();
        SDL_Delay( p_framerate - p_ms );

        int l_framerate = p_fps.getTicks() + p_ms;
        if( (int)p_framerate_list.size() > 20)
            p_framerate_list.erase( p_framerate_list.begin() );

        p_framerate_list.push_back( l_framerate);
    }
    p_fps.start();
}
