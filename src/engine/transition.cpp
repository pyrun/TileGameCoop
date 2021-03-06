#include "transition.h"

transition::transition( graphic* graphic, int deadtime, bool inout) {
    p_image = new image();
    // create surface
    p_image->surface = SDL_CreateRGBSurface( 0, graphic->getCameraSize().x, graphic->getCameraSize().y, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
    SDL_Rect l_screenRect;
    // get rect of screen
    l_screenRect.x = 0;
    l_screenRect.y = 0;
    l_screenRect.w = graphic->getCameraSize().x;
    l_screenRect.h = graphic->getCameraSize().y;
    // fill it with color
    SDL_FillRect( p_image->surface, &l_screenRect, SDL_MapRGB( p_image->surface->format, 0, 0, 0));
    // timer
    p_timer.start();
    // set death time
    p_deadtime = deadtime;

    // out in
    p_inout = inout;

    // create texture form surface
    p_image->texture = SDL_CreateTextureFromSurface( graphic->getRenderer(), p_image->surface);
}

transition::~transition() {
    delete p_image;
}

bool transition::draw( graphic* graphic) {
    // calculate alpha channel
    int l_alpha = 255.f/p_deadtime*p_timer.getTicks();
    if( l_alpha > 255.f)
        l_alpha = 255;

    // set alpha image
    p_image->setAlpha( p_inout==true?255.f-l_alpha:l_alpha );

    // draw image to the screen
    graphic->drawImage( p_image, graphic->getCamera().tovec2(), graphic->getCameraSize(), vec2( 0, 0));

    // look if the time reach death time
    if( p_deadtime < p_timer.getTicks())
        return true;
    return false;
}
