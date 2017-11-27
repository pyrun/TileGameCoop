#ifndef AUDIO_H
#define AUDIO_H

#include "types.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include <string>
#include <vector>




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
        music( Mix_Music *mix, std::string file);
        ~music();

        void play();
        Mix_Music *getPointer() { return p_music; }
        std::string getFileName() { return p_file; }
    protected:
    private:
        std::string p_file;
        Mix_Music *p_music;
};

class audio
{
    public:
        audio( int frequency = 44100, Uint16 format = AUDIO_S16, int channels = 2, int chunksize = 1024);
        virtual ~audio();

        void loadMusic( std::string folder, int volume);

        sound *loadSound( std::string file);

        bool playMusic(std::string file);
    protected:

    private:
        std::vector<music*> p_musics;
        std::string p_last_track;
};

#endif // AUDIO_H
