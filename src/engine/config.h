#ifndef CONFIG_H
#define CONFIG_H

#define CONFIG_FILE "config.xml"

#include <string>

#include "../xml/tinyxml2.h"
#include "types.h"

// debian
#ifdef IMADEBIAN
#include <lua5.3/lua.hpp>
#else
#include <lua.hpp>
#endif // IMADEBIAN

//#define DEBUG_CONFIG

class config
{
    public:
        config();
        virtual ~config();

        int load();
        void save();

        void saveParameter( tinyxml2::XMLDocument *config, tinyxml2::XMLNode *root, std::string name, int data);
        void saveParameter( tinyxml2::XMLDocument *config, tinyxml2::XMLNode *root, std::string name, std::string data);
        int loadParameter( tinyxml2::XMLNode *root, std::string name);
        std::string loadParameterString( tinyxml2::XMLNode *root, std::string name);

        // input
        void setInputPadButton( int run, int jump, int attack, int special, int select, int start, int left, int right) {
            p_gamepad_run = run;
            p_gamepad_jump = jump;
            p_gamepad_attack = attack;
            p_gamepad_special = special;
            p_gamepad_select = select;
            p_gamepad_start = start;
            p_gamepad_left = left;
            p_gamepad_right = right;
        }
        void setInputPadAxis( int x, int y) {
            p_gamepad_x = x;
            p_gamepad_y = y;
        }

        int getInputPadButton_run() { return p_gamepad_run; }
        int getInputPadButton_jump() { return p_gamepad_jump; }
        int getInputPadButton_attack() { return p_gamepad_attack; }
        int getInputPadButton_special() { return p_gamepad_special; }
        int getInputPadButton_select() { return p_gamepad_select; }
        int getInputPadButton_start() { return p_gamepad_start; }
        int getInputPadButton_left() { return p_gamepad_left; }
        int getInputPadButton_right() { return p_gamepad_right; }

        int getInputPadAxisX() { return p_gamepad_x; }
        int getInputPadAxisY() { return p_gamepad_y; }

        bool getQuit() { return p_quit; }

        // display resolution
        void setDisplay( int width, int height) {
            p_display_width = width;
            p_display_height = height;
            p_display_change = true;
        }
        vec2 getDisplay() { return vec2{ p_display_width, p_display_height}; }

        // full screen
        void setDisplayFullscreen( bool mode) { p_display_Fullscreen = mode; }
        bool isDisplayFullscreen()  { return p_display_Fullscreen; }

        // display change ( resize, moved)
        void setDisplayChange() { p_display_change = true; }
        bool isDisplayChanged() { bool l_return = p_display_change; p_display_change = false; return l_return; }

        // change the display mode -> full screen, window mode, maximized
        void setDisplayChangeMode() { p_display_changemode = true; }
        bool isChangeDisplayMode() { bool l_return = p_display_changemode; p_display_changemode = false; return l_return; }

        // display maximized
        void setDisplayMaximized( bool set) { p_display_maximazed = set; }
        bool isDisplayMaximized() { return p_display_maximazed; }

        // display resolution file
        void setDisplayResolutionFile( std::string file) { p_resolution_file = file; }
        std::string getDisplayResolutionFile() { return p_resolution_file; }

        // game rule quit
        void setQuit( bool set) { p_quit = set; };

        // controller map
        void setControllerMappingsFile( std::string file) { p_controllermappingsfile = file; }
        std::string getControllerMappingsFile() { return p_controllermappingsfile; }

        // debug draw flag
        void setDebug( bool set) { p_debug = set; }
        bool getDebug() { return p_debug; }

        // game start file
        std::string getStartfile() { return p_startfile; }
        void setStartfile( std::string file) { p_startfile = file; }

        // force join ( if controller found -> he join the game)
        void setForceJoin( bool set) { p_forceJoin = set; }
        bool getForceJoin() { return p_forceJoin; }

        // music folder
        void setMusicFolder( std::string folder) { p_nusic_folder = folder; }
        std::string getMusicFolder() { return p_nusic_folder; }

        // music volume
        void setMusicVolume( int volume) { p_music_volume = volume; }
        int getMusicVolume() { return p_music_volume; }
    protected:

    private:
        int p_gamepad_run;
        int p_gamepad_jump;
        int p_gamepad_attack;
        int p_gamepad_special;
        int p_gamepad_select;
        int p_gamepad_start;
        int p_gamepad_left;
        int p_gamepad_right;

        int p_gamepad_x;
        int p_gamepad_y;

        int p_display_width;
        int p_display_height;
        bool p_display_change;
        bool p_display_changemode;
        bool p_display_Fullscreen;

        bool p_display_maximazed;

        bool p_quit;

        bool p_debug;

        bool p_forceJoin;


        std::string p_resolution_file;
        std::string p_controllermappingsfile;
        std::string p_startfile;

        std::string p_nusic_folder;
        int p_music_volume;
};

void lua_config_setLink( config *config);
void lua_config_install( lua_State *state);


#endif // CONFIG_H
