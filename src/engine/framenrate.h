#ifndef FRAMENRATE_H
#define FRAMENRATE_H

#include "types.h"

class framenrate
{
    public:
        framenrate( int framerate = 60);
        virtual ~framenrate();

        void begin();
        void calc();
        int getDelay() { return p_ms; }
    protected:

    private:
        int p_framerate; // the framerate what we want
        int p_frame; //Keep track of the current frame
        int p_ms;

        timer p_fps;
};

#endif // FRAMENRATE_H
