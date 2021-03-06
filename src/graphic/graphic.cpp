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
		print( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
		success_initSDL = false;
		SDL_Quit();
	}

	// Start sdl image
	if( success_initSDL && IMG_Init( l_flags) < 0) {
        print( " SDL_Image could not initialize! SDL_IMG_Error: %s\n", IMG_GetError());
        IMG_Quit();
        SDL_Quit();
        success_initSDL = false;
	}

    #ifdef DEBUG
    // This line is only needed, if you want debug the program
    SDL_SetHint(SDL_HINT_WINDOWS_DISABLE_THREAD_NAMING, "1");
    #endif // DEBUG

	return success_initSDL;
}

void pixel_set(SDL_Surface *surface, int x, int y, Uint32 pixel) {
    int bpp = surface->format->BytesPerPixel;
    // Here p is the address to the pixel we want to set
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;
    switch(bpp) {
    case 1:
        *p = pixel;
        break;
    case 2:
        *(Uint16 *)p = pixel;
        break;
    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
            p[0] = (pixel >> 16) & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = pixel & 0xff;
        } else {
            p[0] = pixel & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = (pixel >> 16) & 0xff;
        }
        break;
    case 4:
        *(Uint32 *)p = pixel;
        break;
    }
}

Uint32 pixel_get(SDL_Surface *surface, int x, int y) {
    int bpp = surface->format->BytesPerPixel;
    // Here p is the address to the pixel we want to retrieve
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
    case 1:
        return *p;
        break;
    case 2:
        return *(Uint16 *)p;
        break;
    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            return p[0] << 16 | p[1] << 8 | p[2];
        else
            return p[0] | p[1] << 8 | p[2] << 16;
        break;
    case 4:
        return *(Uint32 *)p;
        break;
    default:
        return 0;
    }
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

    // say free image
    //print( "image::~image free file \"%s\"\n", this->file.c_str());
}

void image::resizeSurface( vec2 size) {
    // create a new surface
    SDL_Surface *l_resize_surface = SDL_CreateRGBSurface(surface->flags, size.x, size.y, surface->format->BitsPerPixel,
        surface->format->Rmask, surface->format->Gmask, surface->format->Bmask, surface->format->Amask);

    // calc factor
    fvec2 l_factor;
    l_factor.x = (static_cast<double>(size.x) / static_cast<double>(surface->w));
    l_factor.y = (static_cast<double>(size.y) / static_cast<double>(surface->h));

    for(int y = 0; y < surface->h; y++)
        for(int x = 0; x < surface->w; x++)
            for(int zoom_y = 0; zoom_y < l_factor.y; ++zoom_y)
                for(int zoom_x = 0; zoom_x < l_factor.x; ++zoom_x)
                    pixel_set(l_resize_surface, (l_factor.x * x) + zoom_x,
                        (l_factor.y * y) + zoom_y, pixel_get(surface, x, y));

    // redraw resize
    SDL_FreeSurface( surface);
    surface = l_resize_surface;
}

/*void image::resizeSurface( vec2 size) {

    surface = ScaleSurface( surface, size.x, size.y);
}*/

graphic::graphic( config *config)
{
    vec2 l_displayResolution;
    // pointer at config
    p_config = config;

    Uint32 l_flags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL;

    if( p_config->get( "fullscreen", "display", "true") == "true") {
        l_flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
    }

    l_displayResolution.x = atoi( p_config->get( "width", "display", "960").c_str() );
    l_displayResolution.y = atoi( p_config->get( "height", "display", "540").c_str() );

    // create window
    p_windows = SDL_CreateWindow( "Jump'n'Run", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, l_displayResolution.x, l_displayResolution.y, l_flags);
    if( p_windows == NULL )
    {
        print( "graphic::graphic Window could not be created! SDL_Error: %s\n", SDL_GetError() );
        success_initSDL = false;
        return;
    }

    // get display res and set
    SDL_GetWindowSize( p_windows, &l_displayResolution.x, &l_displayResolution.y);
    setDisplay( l_displayResolution);

    // creating the renderer
    p_renderer = SDL_CreateRenderer( p_windows, -1, SDL_RENDERER_ACCELERATED );
    if( p_renderer == NULL ) {
        print( "graphic::graphic Renderer could not be created! SDL_Error: %s\n", SDL_GetError() );
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

    // renderer r�umen
    SDL_RenderClear( p_renderer);

    // start the frame count
    p_frame.start();
    p_framecount = 1;

    loadResolution( p_config->get( "resolution_file", "display", "resolution.xml"));

    // windows maxed in config?
    clear();

    // scale set
    if( p_config->get( "fullscreen", "display") == "true")
        setFullscreen( true);
    else
        changeWindowSize(); //p_config->setDisplayChangeMode();

    // set change flag
    p_config->set( "display_change", "true", "game");

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
        print( "graphic::loadResolution file dont exist \"%s\"\n", file.c_str());
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

void graphic::changeWindowSize() {
    int l_width, l_height;
    // get new resolution
    vec2 l_newres;
    l_newres.x = atoi( p_config->get( "width", "display").c_str() );
    l_newres.y = atoi( p_config->get( "height", "display").c_str() );
    // window's client area set
    SDL_SetWindowSize( p_windows, l_newres.x, l_newres.y);

    // set a window's fullscreen state false
    SDL_SetWindowFullscreen( p_windows, 0);

    // set native screen
    p_camera_size.x = NATIV_W;
    p_camera_size.y = NATIV_H;

    // set whether to force integer scales for resolution-independent rendering
    SDL_RenderSetIntegerScale( p_renderer, SDL_TRUE);

    // set device independent resolution
    SDL_RenderSetLogicalSize( p_renderer, NATIV_W, NATIV_H);

    // get zoom factor
    int l_factor = 0;
    for( int i = 0; l_newres.x-i >= NATIV_W; i+=NATIV_W)
        l_factor++;

    // no zero factor
    if( l_factor == 0)
        l_factor = 1;

    // set new window size
    SDL_SetWindowSize( p_windows, NATIV_W*l_factor, NATIV_H*l_factor);

    // set config display
    setDisplay( vec2( NATIV_W*l_factor, NATIV_H*l_factor));

    // get new resolution
    l_newres = vec2( NATIV_W*l_factor, NATIV_H*l_factor);

    // print changed
    print( "graphic::changeWindowSize changed %dx%d zoom factor x%d\n", l_newres.x, l_newres.y, l_factor);
}

void graphic::setFullscreen( bool fromWindow) {
    float l_zoom;
    vec2 l_newr;

    print( "graphic::setFullscreen change\n");

    // window to fullscreen -> set
    if( fromWindow = true) {
        SDL_SetWindowFullscreen( p_windows, SDL_WINDOW_FULLSCREEN_DESKTOP);
        //SDL_SetWindowSize( p_windows, p_config->getDisplay().x, p_config->getDisplay().y);
    }

    // get new resolution
    SDL_GetWindowSize( p_windows, &l_newr.x, &l_newr.y);
    setDisplay( l_newr);

    // calc zoom
    l_zoom = getZoom( vec2( l_newr.x, l_newr.y) );

    // set camera zoom
    p_camera_size.x = l_newr.x/l_zoom;
    p_camera_size.y = l_newr.y/l_zoom;

    // set scale
    SDL_RenderSetScale( p_renderer, (int)l_zoom, (int)l_zoom);
}

void graphic::setDisplay( vec2 resolution) {
    // set display
    p_config->set( "width",  patch::to_string( resolution.x), "display");
    p_config->set( "height",  patch::to_string( resolution.y), "display");
    p_config->set( "display_change", "true", "game");
}

bool graphic::isDisplayChanged( config *config) {
    bool l_chaged = config->get( "display_change", "game", "true") == "true";
    config->set( "display_change", "false", "game");
    return l_chaged;
}

bool graphic::isDisplayChangedMode( config *config) {
    bool l_chaged = config->get( "display_change_mode", "game", "true") == "true";
    config->set( "display_change_mode", "false", "game");
    return l_chaged;
}

#define sdp( a, b) ( sqrt( (a*a) + (b*b) ))

void graphic::clear( float dt) {
    if( p_flyTo ) {
        float l_speed;
        fvec2 l_velocity;
        fvec2 l_change = p_cameraFlyTo - p_camera;

        float l_angle = atan2f( l_change.y, l_change.x);

        l_speed = sdp( l_change.x, l_change.y)/500.f;
        l_speed *= dt;
        l_velocity.x += cos( l_angle) * l_speed;
        l_velocity.y += sin( l_angle) * l_speed;

        if( fabs(l_change.x)+fabs(l_change.y) < l_speed )
            p_camera = p_camera + l_change;
        else p_camera = p_camera + l_velocity;

        p_flyTo = NULL;
    }

    // frame counter
    if( p_frame.getTicks() > FRAME_COUNTER) {
        // increase
        p_framecount++;
        // frame counter
        p_frame.start();
    }

    // react of change
    if( isDisplayChanged( p_config)) {
        SDL_Rect l_viewport;

        l_viewport.x = 0;
        l_viewport.y = 0;
        l_viewport.w = atoi( p_config->get( "width", "display").c_str() );
        l_viewport.h = atoi( p_config->get( "height", "display").c_str() );

        // set render view port
        SDL_RenderSetViewport( p_renderer, &l_viewport);

        // Zoom or change window
        if( isDisplayChangedMode( p_config)) {
            if( p_config->get( "fullscreen", "display") == "false")
                changeWindowSize();
            else
                setFullscreen( true);
        }

        // set config if windows maxmazed
        /*Uint32 l_flags = SDL_GetWindowFlags( p_windows);
        if( l_flags & SDL_WINDOW_MAXIMIZED)
            p_config->setDisplayMaximized( true);
        else
            p_config->setDisplayMaximized( false);*/
    }

    // set background draw color
    SDL_SetRenderDrawColor( p_renderer, 0, 0, 0, 255);

    // display the buffer
    SDL_RenderPresent(p_renderer);

    // clean the buffer
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
            print( "graphic::loadImage Datei \"%s\" konnte nicht zur Textur gemacht werden\n", file.c_str());
            print( "graphic::loadImage SDL_Error: %s\n", SDL_GetError());
            // free the ram data
            SDL_FreeSurface( l_surface);
            l_textureclass = NULL;
        }
    } else {
        print( "graphic::loadImage Datei \"%s\" konnte nicht geladen werden\n", file.c_str());
        print( "graphic::loadImage SDL_Error: %s\n", IMG_GetError());
        l_textureclass = NULL;
    }

    // ausgeben falls detei geladen werde konnte
    //if( l_textureclass != NULL)
    //    print("graphic::loadImage Datei \"%s\" wurde geladen\n", file.c_str());
    // nope
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

void graphic::cutImageFrom( SDL_Surface *srcImage, SDL_Surface *cutImage, vec2 position) {
    // set variable
    SDL_Rect l_sourceRect = { 0, 0, cutImage->w, cutImage->h };
    SDL_Rect l_destRect = { position.x, position.y, cutImage->w, cutImage->h };

    // lock surface to edit
    SDL_LockSurface(srcImage);

    // look at every pixel
    for(int x = 0; x < l_destRect.w; ++x) {
        for(int y = 0; y < l_destRect.h; ++y) {
            // if alpha pixel set source pixel too to alpha
            if( pixel_get( cutImage, x, y) == SDL_MapRGB(cutImage->format, BMP_ALPHA))
                if( l_destRect.x + x > 0 && l_destRect.y + y > 0 && l_destRect.x + x - srcImage->w < 0 && l_destRect.y + y - srcImage->h < 0)
                    pixel_set( srcImage, l_destRect.x + x, l_destRect.y + y, SDL_MapRGB(cutImage->format, BMP_ALPHA));
        }
    }

    // unlock the surface
    SDL_UnlockSurface(srcImage);
}

int graphic::getZoom( vec2 display) {
    for( int i = 0; i < (int)p_resolution.size(); i++) {
        vec2 l_resolution;
        // get resolution
        l_resolution.x = atoi( p_config->get( "width", "display").c_str() );
        l_resolution.y = atoi( p_config->get( "height", "display").c_str() );

        if( l_resolution == p_resolution[i].resolution)
            return p_resolution[i].zoom;
    }
    return 1;
}
