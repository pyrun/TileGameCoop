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
        void loadSound( std::string file);
        void setChunk( Mix_Chunk *chunk) { p_sound = chunk; }
        void setFile( std::string file) { p_file = file; }
        void play() { Mix_PlayChannel( -1, p_sound, 0); }
    protected:

    private:
        std::string p_file;
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
        audio( int frequency = 66150, Uint16 format = AUDIO_S16, int channels = 2, int chunksize = 4096);
        virtual ~audio();

        sound *loadSound( std::string file);
    protected:

    private:
};

#endif // AUDIO_H
