#ifndef TRANSITION_H
#define TRANSITION_H

#include "../graphic/graphic.h"
#include "types.h"
#include "timer.h"

class transition
{
    public:
        transition( graphic* graphic, int deadtime, bool inout);
        virtual ~transition();

        bool draw( graphic* graphic);

        bool blendout() { return p_inout; }
    protected:

    private:
        image *p_image;
        timer p_timer;
        int p_deadtime; // in ms
        bool p_inout;
        bool p_finish;
};

#endif // TRANSITION_H
