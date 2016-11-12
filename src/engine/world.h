#ifndef WORLD_H
#define WORLD_H

#include <string>

#include "../xml/tinyxml2.h"

class world
{
    public:
        world( std::string file);
        virtual ~world();

        bool load( std::string file);
    protected:

    private:
        std::string p_file;

        std::string loadParameter( tinyxml2::XMLNode *root, std::string name);
};

#endif // WORLD_H
