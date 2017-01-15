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

void font::drawMessage(graphic *graphic, std::string text, vec2 pos, bool left, bool up) {
    vec2 l_change;

    // draw all leter now
    for( int i = 0; i < (int)text.size(); i++) {
        // calc change
        if( left == true)
            l_change.x = - ( ( (int)text.size()-i) *FONT_SIZE_W);
        else
            l_change.x = i*FONT_SIZE_W;
        if( up == true)
            l_change.y = -FONT_SIZE_H;

        // draw one letter
        drawLetter( graphic, text.c_str()[i], pos + l_change);
    }
}
