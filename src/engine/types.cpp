#include "types.h"

#include <math.h>

/*double round(double d)
{
  return floor(d + 0.5);
}*/

timer::timer() {
    // Initialize the variables
    stop();
}

void timer::start() {
    // set flag the timer has started
    p_started = true;

    // unpause the timer
    p_paused = false;

    // get the current clock time
    p_startTicks = SDL_GetTicks();
    p_pausedTicks = 0;
}

void timer::stop() {
    // stop the timer
    p_startTicks = 0;
    p_pausedTicks = 0;

    p_started = false;
    p_paused = false;
}

void timer::pause() {
    if( p_started && !p_paused) {
        // pause the timer
        p_paused = true;

        // calculate the paused ticks
        p_pausedTicks = SDL_GetTicks() - p_startTicks;
        p_startTicks = 0;
    }
}

void timer::unpause() {
    // if the timer is running and paused
    if( p_started && p_paused) {
        // unpause the timer
        p_paused = false;

        // reset the starting ticks
        p_startTicks = SDL_GetTicks() - p_pausedTicks;

        // reset the paused ticks
        p_pausedTicks = 0;
    }
}

int timer::getTicks() {
    // the actual timer time
    int l_time = 0;

    // if the timer still running
    if( p_started) {
        if( p_paused) {
            // return the paused time
            l_time = p_pausedTicks;
        } else {
            // return the current time minus the start time
            l_time = SDL_GetTicks() - p_startTicks;
        }
    }

    return l_time;
}

bool file_exist( std::string file) {
    if( access( file.c_str(), F_OK ) != -1 )
        return true;
    return false;
}
