#include "graphic.h"

#include <stdio.h>
#include <SDL2/SDL_image.h>

bool success_initSDL = false;

bool initSDL() {

    // set only once
    if( success_initSDL == true)
        return true;
	//Initialization flag
	bool success_initSDL = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_JOYSTICK ) < 0 )
	{
		printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
		success_initSDL = false;
	}

	return success_initSDL;
}

image::image() {
    // set all to NULL
    surface = NULL;
    texture = NULL;
}

image::~image() {
    // free ram data
    if( surface != NULL)
        SDL_FreeSurface( surface);

    // free vram
    if( texture != NULL)
        SDL_DestroyTexture( texture);
}

graphic::graphic( config *config)
{
    // pointer at config
    p_config = config;

    // create window
    p_windows = SDL_CreateWindow( "Jump'n'Run", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, p_config->getDisplay().x, p_config->getDisplay().y, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if( p_windows == NULL )
    {
        printf( "graphic::graphic Window could not be created! SDL_Error: %s\n", SDL_GetError() );
        success_initSDL = false;
        return;
    }

    // creating the renderer
    p_renderer = SDL_CreateRenderer( p_windows, -1, 0);
    if( p_renderer == NULL ) {
        printf( "graphic::graphic Renderer could not be created! SDL_Error: %s\n", SDL_GetError() );
        success_initSDL = false;
        return;
    }

    // set draf color
    SDL_SetRenderDrawColor( p_renderer, 255, 255, 255, 255);

    // renderer räumen
    SDL_RenderClear( p_renderer);
}

graphic::~graphic()
{
    // destroy renderer
    SDL_DestroyRenderer( p_renderer);

    // destroy window
    SDL_DestroyWindow( p_windows);
}

void graphic::clear() {
    // react of change
    if( p_config->displayChange()) {
        SDL_Rect l_viewport = { 0, 0, p_config->getDisplay().x, p_config->getDisplay().y};
        // transfer data to sdl
        SDL_RenderSetViewport( p_renderer, &l_viewport);
        // set display mode
        SDL_SetWindowFullscreen( p_windows, p_config->getDisplayMode());
    }

    // anzeigen lassen
    SDL_RenderPresent(p_renderer);

    // renderer aufräumen
    SDL_RenderClear( p_renderer);
}

image *graphic::loadImage( std::string file) {
    SDL_Surface *l_surface;
    SDL_Texture *l_texture;
    image *l_textureclass = NULL;

    // load the file in RAM
    l_surface = IMG_Load( file.c_str());

    if( l_surface) {
        // transfer to VRAM
        l_texture = SDL_CreateTextureFromSurface( p_renderer, l_surface);
        if( l_texture) {
            // at the end creating the texture class with all data
            l_textureclass = new image();
            l_textureclass->file = file;
            l_textureclass->surface = l_surface;
            l_textureclass->texture = l_texture;
        } else {
            printf( "graphic::loadImage Datei %s konnte nicht zur Textur gemacht werden\n", file.c_str());
            // free the ram data
            SDL_FreeSurface( l_surface);
        }
    } else {
        printf( "graphic::loadImage Datei %s konnte nicht geladen werden\n", file.c_str());
    }
    return l_textureclass;
}

void graphic::drawImage( image *image, vec2 position, vec2 size, vec2 source, double angle, int flip) {
    SDL_RendererFlip l_flip;

    // chose flip
    switch(flip) {
        case 1: l_flip = SDL_FLIP_HORIZONTAL; break;
        case 2: l_flip = SDL_FLIP_VERTICAL; break;
        case 3: l_flip = (SDL_RendererFlip)(SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL); break;
        case 0:
        default: l_flip = SDL_FLIP_NONE; break;
    }

    // transfer vec to sdl_rect
    SDL_Rect l_source = { source.x, source.y, size.x, size.y};
    SDL_Rect l_destination = { position.x, position.y, size.x, size.y};

    // call the draw fuction of sdl
    SDL_RenderCopyEx( p_renderer, image->texture, &l_source, &l_destination, angle, NULL, l_flip);
}
