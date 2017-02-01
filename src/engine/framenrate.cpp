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
        //p_fps.start();
    }
    p_fps.start();
}
