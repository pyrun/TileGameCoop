#ifndef GRAPHIC_H
#define GRAPHIC_H

#include <SDL2/SDL.h>
#include <string>
#include <vector>

#include "../engine/types.h"
#include "../engine/config.h"
#include "../engine/timer.h"

#define NATIV_W (1920/4)
#define NATIV_H (1080/4)

#define BMP_ALPHA  200, 191, 231

#define FRAME_COUNTER 16 //60hz

bool initSDL(); // call it once

class image {
    public:
        image();
        ~image();

        void setAlpha( int alpha) {  SDL_SetTextureAlphaMod( texture, alpha); }
        void resizeSurface( vec2 size);

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

        int loadResolution( std::string file);
        void changeWindowSize();
        void setFullscreen( bool fromWindow = false);
        void setDisplay( vec2 resolution);
        bool isDisplayChanged( config *config);
        bool isDisplayChangedMode( config *config);
        void clear( float dt = 1.0f);

        image *loadImage( std::string file);
        void drawImage( image *image, vec2 position, vec2 size, vec2 source, double angle = 0.0f, int flip = 0, double factor = 0);

        void cutImageFrom( SDL_Surface *srcImage, SDL_Surface *cutImage, vec2 position);

        SDL_Window *getWindow() { return p_windows; }
        void moveCamera( vec2 move) { p_camera = p_camera + move; }
        void flyTo( vec2 pos) { p_cameraFlyTo = pos; p_flyTo = true; }
        void setCamera( vec2 pos) { p_camera = pos; p_cameraOld = pos; }
        fvec2 getCamera() { return p_cameraOld; }
        vec2 getCameraSize() { return p_camera_size; }
        void flipCamera() { p_cameraOld = p_camera; }
        int getFrame() { return p_framecount; }
        int getZoom( vec2 display);
        SDL_Renderer *getRenderer() { return p_renderer;}
    protected:

    private:
        config *p_config;
        bool p_flyTo;
        fvec2 p_cameraFlyTo;
        fvec2 p_camera;
        fvec2 p_cameraOld;
        vec2 p_camera_size;

        timer p_frame;
        int p_framecount;

        SDL_Window *p_windows;
        SDL_Renderer *p_renderer;

        std::vector<zoom> p_resolution;

};

#endif // GRAPHIC_H
