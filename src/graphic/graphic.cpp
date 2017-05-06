#include "graphic.h"

#include <math.h>
#include <stdio.h>
#include <SDL2/SDL_image.h>

using namespace tinyxml2;

bool success_initSDL = false;

bool initSDL() {
    int l_flags = IMG_INIT_PNG;

    // set only once
    if( success_initSDL == true)
        return true;
	//Initialization flag
	bool success_initSDL = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER) < 0 )
	{
		printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
		success_initSDL = false;
		SDL_Quit();
	}

	// Start sdl image
	if( success_initSDL && IMG_Init( l_flags) < 0) {
        printf( " SDL_Image could not initialize! SDL_IMG_Error: %s\n", IMG_GetError());
        IMG_Quit();
        SDL_Quit();
        success_initSDL = false;
	}

	// This line is only needed, if you want debug the program
    //SDL_SetHint(SDL_HINT_WINDOWS_DISABLE_THREAD_NAMING, "1");

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
    vec2 l_displayResolution;
    // pointer at config
    p_config = config;

    Uint32 l_flags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL;

    if( p_config->getDisplayMode())
        l_flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;

    l_displayResolution = p_config->getDisplay();

    // create window
    p_windows = SDL_CreateWindow( "Jump'n'Run", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, l_displayResolution.x, l_displayResolution.y, l_flags);
    if( p_windows == NULL )
    {
        printf( "graphic::graphic Window could not be created! SDL_Error: %s\n", SDL_GetError() );
        success_initSDL = false;
        return;
    }

    SDL_GetWindowSize( p_windows, &l_displayResolution.x, &l_displayResolution.y);
    p_config->setDisplay( l_displayResolution.x, l_displayResolution.y);

    // creating the renderer
    p_renderer = SDL_CreateRenderer( p_windows, -1, SDL_RENDERER_ACCELERATED );
    if( p_renderer == NULL ) {
        printf( "graphic::graphic Renderer could not be created! SDL_Error: %s\n", SDL_GetError() );
        success_initSDL = false;
        return;
    }

	// camera setings
	p_camera.x = 0;
	p_camera.y = 0;

	//
	p_camera_size = l_displayResolution;

    // set draf color
    SDL_SetRenderDrawColor( p_renderer, 255, 255, 255, 255);

    // renderer räumen
    SDL_RenderClear( p_renderer);

    // start the frame count
    p_frame.start();
    p_framecount = 1;

    loadResolution( p_config->getDisplayResolutionFile());

    // windows maxed in config?
    clear();

    // set change flag
    p_config->setDisplayChange();

    // scale set
    if( p_config->getDisplayMode())
        setFullscreen( );
    else
        p_config->setDisplayChangeMode();
}

graphic::~graphic()
{
    // destroy renderer
    SDL_DestroyRenderer( p_renderer);

    // destroy window
    SDL_DestroyWindow( p_windows);
}

int graphic::loadResolution( std::string file) {
    XMLDocument l_file;

    // load form world file
    l_file.LoadFile( file.c_str());

    // file exist?
    if( file_exist( file) == false) {
        printf( "graphic::loadResolution file dont exist \"%s\"\n", file.c_str());
        return 1;
    }

    XMLElement* l_xml_resolution = l_file.FirstChildElement( "display" );

    // load all resolution
    while( l_xml_resolution) {
        zoom l_zoom;

        // load data
        l_zoom.zoom = atoi( l_xml_resolution->Attribute("zoom") == NULL?"1":l_xml_resolution->Attribute("zoom") );
        l_zoom.resolution.x = atoi(l_xml_resolution->Attribute( "x"));
        l_zoom.resolution.y = atoi(l_xml_resolution->Attribute( "y"));

        p_resolution.push_back( l_zoom);

        // next object
        l_xml_resolution = l_xml_resolution->NextSiblingElement( "display");
    }

    return 0;
}

#define sdp( a, b) ( sqrt( (a*a) + (b*b) ))

void graphic::changeWindowSize() {
    vec2 l_newres;
    SDL_GetWindowSize( p_windows, &l_newres.x, &l_newres.y);
    SDL_SetWindowFullscreen( p_windows, 0);
    p_camera_size.x = NATIV_W;
    p_camera_size.y = NATIV_H;
    SDL_RenderSetLogicalSize( p_renderer, NATIV_W, NATIV_H);
    printf( "graphic::changeWindowSize changed %dx%d\n", l_newres.x, l_newres.y);
}

void graphic::setFullscreen( bool fromWindow) {
    float l_zoom;
    vec2 l_newr;

    printf( "graphic::setFullscreen change\n");
    // window to fullscreen -> set
    if( fromWindow = true) {
        SDL_SetWindowFullscreen( p_windows, SDL_WINDOW_FULLSCREEN_DESKTOP);
        //SDL_SetWindowSize( p_windows, p_config->getDisplay().x, p_config->getDisplay().y);
    }

    // get new resolution
    SDL_GetWindowSize( p_windows, &l_newr.x, &l_newr.y);
    p_config->setDisplay( l_newr.x, l_newr.y);

    // calc zoom
    l_zoom = getZoom( p_config->getDisplay() );

    // set camera zoom
    p_camera_size.x = p_config->getDisplay().x/l_zoom;
    p_camera_size.y = p_config->getDisplay().y/l_zoom;

    // set scale
    SDL_RenderSetScale( p_renderer, (int)l_zoom, (int)l_zoom);
}

void graphic::clear( float dt) {
    if( p_flyTo ) {
        float l_speed;
        fvec2 l_velocity;
        fvec2 l_change = p_cameraFlyTo - p_camera;

        float l_angle = atan2f( l_change.y, l_change.x);

        l_speed = sdp( l_change.x, l_change.y)/100.f;
        l_speed *= dt;
        l_velocity.x += cos( l_angle) * l_speed;
        l_velocity.y += sin( l_angle) * l_speed;

        if( fabs(l_change.x)+fabs(l_change.y) < l_speed )
            p_camera = p_camera + l_change;
        else p_camera = p_camera + l_velocity;

        p_flyTo = NULL;
    }

    // fame count
    if( p_frame.getTicks() > FRAME) {
        p_framecount++;
        p_frame.start();
    }

    // react of change
    if( p_config->displayChange()) {
        SDL_Rect l_viewport = { 0, 0, p_config->getDisplay().x, p_config->getDisplay().y};

        //p_camera_size = p_config->getDisplay();


        // transfer data to sdl
        SDL_RenderSetViewport( p_renderer, &l_viewport);

        // Zoom or change window
        if( p_config->getDisplayChangeMode()) {
            if( !p_config->getDisplayMode())
                changeWindowSize();
            else
                setFullscreen( true);
        }


        // set config if windows maxmazed
        Uint32 l_flags = SDL_GetWindowFlags( p_windows);
        if( l_flags & SDL_WINDOW_MAXIMIZED)
            p_config->setDisplayMaximized( true);
        else
            p_config->setDisplayMaximized( false);
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

    SDL_SetColorKey( l_surface, SDL_TRUE, SDL_MapRGB(l_surface->format, BMP_ALPHA));

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
            printf( "graphic::loadImage SDL_Error: %s\n", SDL_GetError());
            // free the ram data
            SDL_FreeSurface( l_surface);
            l_textureclass = NULL;
        }
    } else {
        printf( "graphic::loadImage Datei %s konnte nicht geladen werden\n", file.c_str());
        printf( "graphic::loadImage SDL_Error: %s\n", IMG_GetError());
        l_textureclass = NULL;
    }

    // ausgeben falls detei geladen werde konnte
    if( l_textureclass != NULL)
        printf("graphic::loadImage Datei %s wurde geladen\n", file.c_str());
    return l_textureclass;
}

void graphic::drawImage( image *image, vec2 position, vec2 size, vec2 source, double angle, int flip, double factor) {
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
    SDL_Rect l_destination = { position.x - getCamera().x, position.y - getCamera().y, size.x, size.y};

    if( factor != 0.0 ) {
        l_source.w = l_source.w*factor;
        l_source.h = l_source.h*factor;
        l_destination.w = l_source.w/factor;
        l_destination.h = l_source.h/factor;

        if( l_destination.w > image->surface->w/factor )
            l_destination.w = image->surface->w/factor;
        if( l_destination.h > image->surface->h/factor )
             l_destination.h = image->surface->h/factor;
    }

    // call the draw fuction of sdl
    SDL_RenderCopyEx( p_renderer, image->texture, &l_source, &l_destination, angle, NULL, l_flip);
}

int graphic::getZoom( vec2 display) {
    for( int i = 0; i < (int)p_resolution.size(); i++) {
        if( p_config->getDisplay() == p_resolution[i].resolution)
            return p_resolution[i].zoom;
    }
    return 1;
}
