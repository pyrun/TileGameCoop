#ifndef CONFIG_H
#define CONFIG_H

#define CONFIG_FILE "config.xml"

#include <string>

#include "../xml/tinyxml2.h"
#include "types.h"

//#define DEBUG_CONFIG 1

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
        // graphic
        void setDisplay( int width, int height) {
            p_display_width = width;
            p_display_height = height;
            p_display_change = true;
        }
        void setDisplayMode( int mode) {
            p_display_mode = mode;
        }
        void setDisplayChange() { p_display_change = true; }

        vec2 getDisplay() { return vec2{ p_display_width, p_display_height}; }
        int getDisplayMode()  { return p_display_mode; }
        bool displayChange() { bool l_return = p_display_change; p_display_change = false; return l_return; }
        bool getDisplayChangeMode() { bool l_return = p_display_changemode; p_display_changemode = false; return l_return; }

        void setDisplayMaximized( bool set) {
            p_display_maximazed = set;
        }

        bool getDisplayMaximized() { return p_display_maximazed; }
        void setQuit( bool set) { p_quit = set; };
        void setDisplayResolutionFile( std::string file) { p_resolution_file = file; }
        void setDisplayChangeMode() { p_display_changemode = true; }
        std::string getDisplayResolutionFile() { return p_resolution_file; }
        void setControllerMappingsFile( std::string file) { p_controllermappingsfile = file; }
        std::string getControllerMappingsFile() { return p_controllermappingsfile; }
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
        int p_display_mode;

        bool p_display_maximazed;

        bool p_quit;


        std::string p_resolution_file;
        std::string p_controllermappingsfile;
};

#endif // CONFIG_H
