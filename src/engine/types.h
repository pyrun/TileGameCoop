#ifndef TYPES_H
#define TYPES_H

#include <SDL2/SDL_timer.h>

struct vec2 {
    vec2( int x = 0, int y = 0) {
        this->x = x;
        this->y = y;
    }
    int x;
    int y;
};

struct vec4 {
    vec4( int x = 0, int y = 0, int z = 0, int w = 0) {
        this->x = x;
        this->y = y;
        this->z = z;
        this->w = w;
    }
    int x;
    int y;
    int z;
    int w;
};

// timer
class timer {
    public:
        timer();

        // clonk actions
        void start();
        void stop();
        void pause();
        void unpause();

        // get the current ticks of this timer
        int getTicks();

    private:
        // the clock time
        int p_startTicks;

        // the ticks stored when the timer was pause
        int p_pausedTicks;

        // timer status
        bool p_paused;
        bool p_started;
};

/*class types
{
    public:
        types();
        virtual ~types();

    protected:

    private:
};*/

#endif // TYPES_H
