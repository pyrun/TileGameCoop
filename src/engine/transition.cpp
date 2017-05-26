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

    p_finish = false;

    // out in
    p_inout = inout;

    // create texture form surface
    p_image->texture = SDL_CreateTextureFromSurface( graphic->getRenderer(), p_image->surface);
}

transition::~transition() {
    delete p_image;
    //dtor
}

bool transition::draw( graphic* graphic) {
    /*vec2 l_size = vec2(200+1*p_timer.getTicks(), 200+1*p_timer.getTicks());

    p_cutimage->resizeSurface(  l_size);*/

    int l_alpha = 255.f/p_deadtime*p_timer.getTicks();
    if( l_alpha > 255.f)
        l_alpha = 255;

    p_image->setAlpha( p_inout==true?255.f-l_alpha:l_alpha );

    //vec2 l_pos;

    //l_pos.x = cos(p_timer.getTicks())*(p_timer.getTicks()/10);
    //l_pos.y = sin(p_timer.getTicks())*(p_timer.getTicks()/10);

    //graphic->cutImageFrom( p_temp_screen, p_cutimage->surface, l_pos+vec2( graphic->getCameraSize().x/2, graphic->getCameraSize().y/2) );

    //SDL_RenderCopy( graphic->getRenderer(), l_tex, NULL, &p_screenRect);
    graphic->drawImage( p_image, graphic->getCamera().tovec2(), graphic->getCameraSize(), vec2( 0, 0));

    // look if the time reach death time
    if( p_deadtime < p_timer.getTicks()) {
        p_finish = true;
        return true;
    }
    return false;
}
