#include "audio.h"

#include <stdio.h>

#include <dirent.h>

float calculate_distance(vec2 p1, vec2 p2) {
    float diffY = p1.y - p2.y;
    float diffX = p1.x - p2.x;
    return sqrt((diffY * diffY) + (diffX * diffX));
}

sound::sound()
{
    p_sound = NULL;

    // todo
    setCameraPosition( vec2( 0, 0) );
}

sound::~sound()
{
    // free the sample
    if( p_sound != NULL) {
        printf( "sound::~sound free \"%s\" sound\n", p_file.c_str());
        Mix_FreeChunk( p_sound);
    }
}

void sound::loadSound( std::string file) {
    Mix_Chunk *l_chunk;

    // load chunk
    l_chunk = Mix_LoadWAV( file.c_str());
    if(!l_chunk) {
        printf("sound::loadSound Mix_LoadWAV: %s\n", Mix_GetError());
    } else {
        // creating sound object
        setChunk( l_chunk);
        setFile( file);
    }
}

#define SOUND_NEAR_LEVEL 570.f
#define SOUND_MAX_VOLUME 240 // 255 is max! WARNIG
#define SOUND_DISTANCE_FACTOR 1.5f

void sound::play( int volume, vec2 position) {
    float l_volume = (float)MIX_MAX_VOLUME/100.f*volume;
    int l_channel = Mix_PlayChannel( -1, p_sound, 0);
    Mix_Volume( l_channel, l_volume);

    float t_distance = calculate_distance( position, p_camera) * SOUND_DISTANCE_FACTOR;
    int t_volume = 0; // 255 muted -> over 255 random stuff

    // if the
    if( t_distance > SOUND_NEAR_LEVEL)
        t_volume = t_distance-SOUND_NEAR_LEVEL;

    // distance when sound volume go down
    if( t_volume > SOUND_MAX_VOLUME)
        t_volume = SOUND_MAX_VOLUME;

    Mix_SetDistance( l_channel, t_volume);
}

music::music( Mix_Music *mix, std::string file)
{
    p_music = mix;
    p_file = file;
}

music::~music()
{
}

audio::audio( int frequency, Uint16 format, int channels, int chunksize)
{
    int l_flags = MIX_INIT_OGG|MIX_INIT_MP3;

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

audio::~audio()
{
    // clear
    for( int i = 0; i < (int)p_musics.size(); i++) {
        Mix_FreeMusic( p_musics[i]->getPointer());
        p_musics.erase( p_musics.begin() + i);
        i--;
    }
    p_musics.clear();
    // quit SDL_mixer
	Mix_CloseAudio();
}

void audio::loadMusic( std::string folder, int volume) {
    DIR *l_dir;

    struct dirent *l_entry;

    std::string l_path = folder;

    l_dir = opendir(l_path.c_str());
    if ( l_dir == NULL) {  /* error opening the directory? */
        printf("audio::loadMusic cant load music, dir not found\n");
    }

    while ((l_entry = readdir(l_dir)) != NULL) {
        std::string l_file = l_path + l_entry->d_name;
        std::string l_end = l_file.substr(l_file.find_last_of(".") + 1);

        if( l_end == "mp3" || l_end == "ogg") {
            Mix_Music *l_music;
            music *l_musicEntity;

            // try load music
            l_music=Mix_LoadMUS( l_file.c_str());
            // error handling
            if( !l_music) {
                printf("audio::loadMusic: %s\n", Mix_GetError());
                return;
            }

            // print we found one
            //printf("audio::loadMusic: music \"%s\" ", l_file.c_str());

            // save link
            l_musicEntity = new music( l_music, l_entry->d_name);

            // print we found one
            //printf("loaded\n", l_file.c_str());

            // push back entity
            p_musics.push_back( l_musicEntity);
        }
    }
    closedir(l_dir);

    // calculate volume
    float l_volume = (float)MIX_MAX_VOLUME/100.f*volume;

    // set volume of music
    Mix_VolumeMusic( l_volume);
}

sound *audio::loadSound( std::string file) {
    sound *l_sound = new sound();
    // load
    l_sound->loadSound( file);
    // return
    return l_sound;
}

bool audio::playMusic(std::string file) {
    music *l_music = NULL;

    // check if no music given
    if( file == "" && file != p_last_track) {
        // fade out the music
        Mix_FadeOutMusic( 1000);
        p_last_track = "";
        // found answer 42
        return true;
    }

    // search for music and play it
    for( int i = 0; i < (int)p_musics.size(); i++ ) {
        music* l_music = p_musics[ i];
        if( l_music->getFileName() == file && p_last_track != l_music->getFileName()) {
            // fade in music
            Mix_FadeInMusic( l_music->getPointer(), -1, 2000);

            // starting the music
            //Mix_PlayMusic( l_music->getPointer(), -1);

            // save last point
            p_last_track = l_music->getFileName();
            return true;
        }
    }

    // nothing found
    return false;
}
