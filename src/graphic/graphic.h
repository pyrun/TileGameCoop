#ifndef GRAPHIC_H
#define GRAPHIC_H

#include <SDL2/SDL.h>
#include <string>
#include <vector>

#include "../engine/config.h"


#define NATIV_W (1920/4)
#define NATIV_H (1080/4)

#define BMP_ALPHA  200, 191, 231

#define FRAME 16 //60hz

bool initSDL(); // call it once

class image {
    public:
        image();
        ~image();

        SDL_Texture *texture;
        SDL_Surface *surface;
        std::string file;
};

class zoom {
    public:
        vec2 resolution;
        int zoom;
};

class graphic
{
    public:
        graphic( config *config);
        virtual ~graphic();

        void loadResolution( std::string file);
        void clear();

        image *loadImage( std::string file);
        void drawImage( image *image, vec2 position, vec2 size, vec2 source, double angle = 0.0f, int flip = 0, double factor = 0);

        SDL_Window *getWindow() { return p_windows; }
        void moveCamera( vec2 move) { p_camera = p_camera + move; }
        void setCamera( vec2 pos) { p_camera = pos; }
        vec2 getCamera() { return p_cameraOld; }
        vec2 getCameraSize() { return p_camera_size; }
        void flipCamera() { p_cameraOld = p_camera; }
        int getFrame() { return p_framecount; }
        int getZoom( vec2 display);
    protected:

    private:
        config *p_config;
        vec2 p_camera;
        vec2 p_cameraOld;
        vec2 p_camera_size;

        timer p_frame;
        int p_framecount;

        SDL_Window *p_windows;
        SDL_Renderer *p_renderer;

        std::vector<zoom> p_resolution;

};

#endif // GRAPHIC_H
