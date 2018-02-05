#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <vector>

#define CONFIG_FILE "config.xml"

#include "../xml/tinyxml2.h"
#include "lua_loader.h"
#include "types.h"

class config_entity {
    public:
        std::string id;
        std::string data;
        std::string category;

        bool operator < (const config_entity& obj) const {
            return ( category < obj.category);
        }
};

class config {
    public:
        config();
        virtual ~config();
        void set( std::string id, std::string data, std::string category = "");
        std::string get( std::string id, std::string category, std::string defaultValue = "default");

        void save();
        bool load();

        void save_parameter( tinyxml2::XMLDocument *config, tinyxml2::XMLNode *root, std::string name, std::string data);
        std::string load_parameter( tinyxml2::XMLNode *root, std::string name);
    protected:

    private:
        std::vector<config_entity> p_entity;
};

#endif // CONFIG_H
