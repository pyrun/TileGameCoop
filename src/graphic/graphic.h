#ifndef GRAPHIC_H
#define GRAPHIC_H

#include <SDL2/SDL.h>
#include <string>

#include "../engine/config.h"

#define NATIV_ZOOM 4

#define NATIV_W (1920/NATIV_ZOOM)
#define NATIV_H (1080/NATIV_ZOOM)

#define BMP_ALPHA  200, 191, 231

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
        void drawImage( image *image, vec2 position, vec2 size, vec2 source, double angle = 0.0f, int flip = 0, double factor = 0);

        SDL_Window *getWindow() { return p_windows; }
        void moveCamera( vec2 move) { p_camera = p_camera + move; }
        void setCamera( vec2 pos) { p_camera = pos; }
        vec2 getCamera() { return p_cameraOld; }
        vec2 getCameraSize() { return p_camera_size; }
        void flipCamera() { p_cameraOld = p_camera; }
    protected:

    private:
        config *p_config;
        vec2 p_camera;
        vec2 p_cameraOld;
        vec2 p_camera_size;

        SDL_Window *p_windows;
        SDL_Renderer *p_renderer;

};

#endif // GRAPHIC_H
