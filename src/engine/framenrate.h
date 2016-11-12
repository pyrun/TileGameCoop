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
    protected:

    private:
        int p_framerate; // the framerate what we want
        int p_frame; //Keep track of the current frame

        timer p_fps;
};

#endif // FRAMENRATE_H
