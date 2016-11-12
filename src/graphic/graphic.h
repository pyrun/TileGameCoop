#ifndef GRAPHIC_H
#define GRAPHIC_H

#include <SDL2/SDL.h>
#include <string>

#include "../engine/config.h"

bool initSDL(); // call it once

class image {
    public:
        image();
        ~image();

        SDL_Texture *texture;
        SDL_Surface *surface;
        std::string file;
};

class graphic
{
    public:
        graphic( config *config);
        virtual ~graphic();

        void clear();

        image *loadImage( std::string file);
        void drawImage( image *image, vec2 position, vec2 size, vec2 source, double angle, int flip);

        SDL_Window *getWindow() { return p_windows; }
    protected:

    private:
        config *p_config;

        SDL_Window *p_windows;
        SDL_Renderer *p_renderer;

};

#endif // GRAPHIC_H
