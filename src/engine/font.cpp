#include "font.h"

font::font(graphic *graphic)
{
    p_font = graphic->loadImage( FONT_PATH);
    //ctor
}

font::~font()
{
    //dtor
}
