#ifndef AUDIO_H
#define AUDIO_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include <string>

class sound
{
     public:
        sound();
        ~sound();
        void setChunk( Mix_Chunk *chunk) { p_sound = chunk; }
        void play() { Mix_PlayChannel( -1, p_sound, 0); }
    protected:

    private:
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
        audio( int frequency = 22050, Uint16 format = MIX_DEFAULT_FORMAT, int channels = 2, int chunksize = 4096);
        virtual ~audio();

        sound *loadSound( std::string file);
    protected:

    private:
};

#endif // AUDIO_H
