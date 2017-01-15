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
        void drawMessage(graphic *graphic, std::string text, vec2 pos, bool left = false, bool up = false);
        void drawLetter(graphic *graphic, char ascii, vec2 pos) {
            graphic->drawImage( p_font, pos, vec2(FONT_SIZE_W, FONT_SIZE_H),vec2( getXFont( ascii), getYFont( ascii)) );
        }
    protected:
    private:
        int getYFont( char ascii) { return (ascii - 32) / 16 * 12; }
        int getXFont( char ascii) { return (ascii - 32) % 16 * 8; }

        image *p_font;
};

#endif // FONT_H
