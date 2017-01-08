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
}

framenrate::~framenrate()
{
    //dtor
}

void framenrate::begin() {
    // start the frame timer
    p_fps.start();
}

void framenrate::calc() {
    // Increment the frame counter
    p_frame++;

    // ms
    p_ms = p_fps.getTicks();

    if( p_fps.getTicks() < 1000.f/ p_framerate)
        SDL_Delay( ( 1000.f/ p_framerate) - p_fps.getTicks() );
}
