#ifndef INPUT_H
#define INPUT_H

#include <SDL2\SDL.h>

#include "types.h"
#include "config.h"

class input_map {
    public:
        input_map();
        ~input_map() {}
        int x;
        int y;
        bool run;
        bool jump;
        bool attack;
        bool special;
        bool select;
        bool start;
        bool left;
        bool right;
};

class input
{
    public:
        input( config *config);
        virtual ~input();

        bool handle( SDL_Window* window);

        bool flagResize() { return p_resize; }
        vec2 getResizeDisplay() { return vec2{ p_display_width, p_display_height}; }
    protected:

    private:
        SDL_Event p_event;
        config *p_config;

        bool p_resize;
        int p_display_width;
        int p_display_height;
};

#endif // INPUT_H
