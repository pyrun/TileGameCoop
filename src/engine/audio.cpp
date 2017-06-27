#include "audio.h"

#include <stdio.h>

sound::sound()
{
    p_sound = NULL;
}

sound::~sound()
{
    //dtor
}

music::music()
{
    p_music = NULL;
}

music::~music()
{
    //dtor
}

audio::audio( int frequency, Uint16 format, int channels, int chunksize)
{
    int l_flags = MIX_INIT_OGG;

    // load support for the sample/music formats
    int l_init_flags = Mix_Init( l_flags);
    if( l_init_flags&l_flags != l_flags) {
        printf( "audio::audio Mix_Init failed %s\n", Mix_GetError());
		return;
    }

    //Initialize SDL_mixer
	if( Mix_OpenAudio( frequency, format, channels, chunksize ) == -1 ) {
		printf( "audio::audio Mix_OpenAudio failed\n");
		return;
	}
}

sound *audio::loadSound( std::string file) {
    Mix_Chunk *l_chunk;
    sound *l_sound = NULL;

    // load chunk
    l_chunk = Mix_LoadWAV( file.c_str());
    if(!l_chunk) {
        printf("audio::loadSound Mix_LoadWAV: %s\n", Mix_GetError());
    } else {
        // creating sound object
        l_sound = new sound();
        l_sound->setChunk( l_chunk);
    }
    return l_sound;
}

audio::~audio()
{
    // quit SDL_mixer
	Mix_CloseAudio();
}
