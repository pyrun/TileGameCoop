#ifndef TRANSITION_H
#define TRANSITION_H

#include "../graphic/graphic.h"

class transition
{
    public:
        transition( graphic* graphic);
        virtual ~transition();

        void draw( graphic* graphic);
    protected:

    private:
        SDL_Surface* p_temp_screen;
        SDL_Rect p_screenRect;
};

#endif // TRANSITION_H
