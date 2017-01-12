#ifndef WORLD_H
#define WORLD_H

#include <string>
#include <vector>

#include "../xml/tinyxml2.h"
#include "../graphic/graphic.h"

class tiletype {
    public:
        std::vector<int> id;
        int speed;
};

class tile {
    public:
        int id;
        int speed;
        tiletype *type;
};

class world_background {
    public:
        world_background() {
            picture = NULL;
        }
        std::string name;
        std::string picture_file;
        image *picture;
};

class world
{
    public:
        world( std::string file, std::string ordner );
        virtual ~world();

        void loadTypes( std::string file);
        bool load( std::string file, std::string ordner);
        tiletype *findType( int id);
        tile *getTile( tile *tilemap, int x, int y);
        int getTypeIndex( int id, tiletype *type);
        void addBackground( tinyxml2::XMLElement* background, std::string ordner);
        void drawTile( graphic *graphic, int x, int y, tile *map);
        void drawBackAndForeground( graphic *graphic);
        void drawOverground( graphic *graphic);
        void draw( graphic *graphic);
        float getGravity() { return p_gravity; }
    protected:

    private:
        std::string p_file;
        std::string loadParameter( tinyxml2::XMLNode *root, std::string name);

        tile *readTilemap( std::string tilemap);

        tile *p_tilemap_overlap;
        tile *p_tilemap_foreground;
        tile *p_tilemap_background;

        int p_map_width;
        int p_map_hight;

        int p_tilewidth;
        int p_tilehight;

        std::string p_tilesetpath;
        image *p_tileset;
        int p_tileset_width;
        int p_tileset_height;
        int p_tileset_space;
        float p_gravity;

        std::vector<world_background> p_backgrounds;

        std::vector<tiletype> p_tiletypes;
};

#endif // WORLD_H
