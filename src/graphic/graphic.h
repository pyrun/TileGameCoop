#ifndef GRAPHIC_H
#define GRAPHIC_H

#include <SDL2/SDL.h>
#include <string>

#include "../engine/config.h"

#define NATIV_W 640
#define NATIV_H 400

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
        void drawImage( image *image, vec2 position, vec2 size, vec2 source, double angle = 0.0f, int flip = 0);

        SDL_Window *getWindow() { return p_windows; }
        void moveCemara( vec2 move) { p_camera = p_camera + move; }
        vec2 getCamera() { return p_camera; }
        vec2 getCameraSize() { return p_camera_size; }
    protected:

    private:
        config *p_config;
        vec2 p_camera;
        vec2 p_camera_size;

        SDL_Window *p_windows;
        SDL_Renderer *p_renderer;

};

#endif // GRAPHIC_H
