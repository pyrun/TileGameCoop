#ifndef TYPES_H
#define TYPES_H

#include <SDL2/SDL_timer.h>
#include <math.h>
#include <unistd.h>
#include <string>

struct vec2 {
    vec2( int x = 0, int y = 0) {
        this->x = x;
        this->y = y;
    }
    int x;
    int y;

    vec2 operator+( const vec2 &a) {
        vec2 c;
        c.x = x + a.x;
        c.y = y + a.y;
        return c;
    }

    vec2 operator-( const vec2 &a) {
        vec2 c;
        c.x = x - a.x;
        c.y = y - a.y;
        return c;
    }

    bool operator==( const vec2& a) {
        if( a.x == x && a.y == y)
            return true;
        return false;
    }
};

struct vec4 {
    vec4( int x = 0, int y = 0, int z = 0, int w = 0) {
        this->x = x;
        this->y = y;
        this->z = z;
        this->w = w;
    }
    int x;
    int y;
    int z;
    int w;
};

struct fvec2 {
    fvec2( float x = 0, float y = 0) {
        this->x = x;
        this->y = y;
    }
    float x;
    float y;

    fvec2 operator+( const fvec2 &a) {
        fvec2 c;
        c.x = x + a.x;
        c.y = y + a.y;
        return c;
    }

    fvec2 operator-( const fvec2 &a) {
        fvec2 c;
        c.x = x - a.x;
        c.y = y - a.y;
        return c;
    }

    fvec2 operator=( const vec2 &a) {
        x = a.x;
        y = a.y;
        return *this;
    }

    vec2 tovec2() {
        vec2 c;
        c.x = (int)round( (double)this->x);
        c.y = (int)round( (double)this->y);
        return c;
    }
};

// timer
class timer {
    public:
        timer();

        // clonk actions
        void start();
        void stop();
        void pause();
        void unpause();

        // get the current ticks of this timer
        int getTicks();

    private:
        // the clock time
        int p_startTicks;

        // the ticks stored when the timer was pause
        int p_pausedTicks;

        // timer status
        bool p_paused;
        bool p_started;
};


extern bool file_exist( std::string file);

/*class types
{
    public:
        types();
        virtual ~types();

    protected:

    private:
};*/

#endif // TYPES_H
