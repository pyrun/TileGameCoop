#ifndef INPUT_H
#define INPUT_H

#include <SDL2/SDL.h>

#include "types.h"
#include "config.h"

#include <vector>

class input_dir {
    public:
        bool right;
        bool left;
        bool up;
        bool down;
};

class input_map {
    public:
        input_map();
        ~input_map() {}
        int x;
        int y;
        input_dir dir;

        bool run;bool dir_right;

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
        std::vector<int> getDevice() { return p_device; }
        std::vector<int> getDeviceDestroyed() { return p_deviceDestroyed; }
        void cearDevice() { p_device.clear(); p_deviceDestroyed.clear(); };
    protected:

    private:
        SDL_Event p_event;
        config *p_config;

        bool p_resize;
        int p_display_width;
        int p_display_height;

        std::vector<int> p_device;
        std::vector<int> p_deviceDestroyed;
};

#endif // INPUT_H
