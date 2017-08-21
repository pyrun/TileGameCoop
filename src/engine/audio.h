#ifndef AUDIO_H
#define AUDIO_H

#include "types.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include <string>




class sound
{
     public:
        sound();
        ~sound();
        void loadSound( std::string file);
        void setChunk( Mix_Chunk *chunk) { p_sound = chunk; }
        void setFile( std::string file) { p_file = file; }
        void play( int volume, vec2 position);
        void setCameraPosition( vec2 pos) {
            p_camera = pos;
        }
    protected:

    private:
        std::string p_file;
        vec2 p_camera;
        Mix_Chunk *p_sound;
};

class music
{
     public:
        music();
        ~music();

    protected:

    private:
        Mix_Chunk *p_music;
};

class audio
{
    public:
        audio( int frequency = 66150, Uint16 format = AUDIO_S16, int channels = 2, int chunksize = 1024);
        virtual ~audio();

        sound *loadSound( std::string file);
    protected:

    private:
};

#endif // AUDIO_H
