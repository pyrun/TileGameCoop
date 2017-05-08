#ifndef FONT_H
#define FONT_H

#define FONT_PATH "font/font.png"
#define FONT_SIZE_W 8
#define FONT_SIZE_H 12

#include "../graphic/graphic.h"

class font
{
    public:
        font( graphic *graphic);
        virtual ~font();
        void drawMessage(graphic *graphic, std::string text, vec2 pos, float zoom = 1.0f, int alpha = 255, bool left = false, bool up = false);
        void drawLetter(graphic *graphic, char ascii, vec2 pos, float l_factor = 1) {
            graphic->drawImage( p_font, pos, vec2(FONT_SIZE_W/l_factor, FONT_SIZE_H/l_factor),vec2( getXFont( ascii), getYFont( ascii)), 0, 0, l_factor );
        }
    protected:
    private:
        int getYFont( char ascii) { return (ascii - 32) / 16 * 12; }
        int getXFont( char ascii) { return (ascii - 32) % 16 * 8; }

        image *p_font;
};

#endif // FONT_H
