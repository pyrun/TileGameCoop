#ifndef TIMER_H
#define TIMER_H


#include <SDL2/SDL_timer.h>

// timer
class timer {
    public:
        timer();

        // clonk actions
        void start();
        void stop();
        void pause();
        void unpause();

        bool isStarted() { return p_started; }

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

#endif // TIMER_H
