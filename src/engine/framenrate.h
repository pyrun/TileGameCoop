#ifndef FRAMENRATE_H
#define FRAMENRATE_H

#include "types.h"
#include <vector>

class framenrate
{
    public:
        framenrate( int framerate = 16);
        virtual ~framenrate();

        void begin();
        void calc();
        int getDelay() {
            float l_ms = 0;

            for( int i =0; i < (int)p_ms_list.size(); i++)
                l_ms += p_ms_list[i];
            if( l_ms != 0)
                l_ms = l_ms/((float)p_ms_list.size());
            return (int)l_ms;
        }
        int getFramerate() {
            float l_framerate = 0;

            for( int i =0; i < (int)p_framerate_list.size(); i++)
                l_framerate += p_framerate_list[i];
            if( l_framerate != 0)
                l_framerate = l_framerate/((float)p_framerate_list.size());
            return (int)l_framerate;
        }
        int getMax() { return p_framerate; }
    protected:

    private:
        int p_framerate; // the framerate what we want
        int p_frame; //Keep track of the current frame
        int p_ms;

        timer p_fps;

        std::vector<int> p_ms_list;
        std::vector<int> p_framerate_list;
};

#endif // FRAMENRATE_H
