#include "transition.h"

transition::transition( graphic* graphic) {
    // create surface
    p_temp_screen = SDL_CreateRGBSurface( 0, graphic->getCameraSize().x, graphic->getCameraSize().y, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
    // get rect of screen
    p_screenRect.x = 0;
    p_screenRect.y = 0;
    p_screenRect.w = graphic->getCameraSize().x;
    p_screenRect.h = graphic->getCameraSize().y;
    // fill it with color
    SDL_FillRect( p_temp_screen, &p_screenRect, 0xFF202020);
}

transition::~transition()
{
    SDL_FreeSurface( p_temp_screen);
    //dtor
}

void transition::draw( graphic* graphic) {
    SDL_Texture* l_temp_screen_texture = SDL_CreateTextureFromSurface( graphic->getRenderer(), p_temp_screen);

    SDL_RenderCopy( graphic->getRenderer(), l_temp_screen_texture, NULL, &p_screenRect);

    SDL_DestroyTexture( l_temp_screen_texture);
}
