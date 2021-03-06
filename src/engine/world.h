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
        bool top;
        bool down;
        bool left;
        bool right;
        bool liquid;
};

class tile {
    public:
        tile() { type = NULL; }

        int id;
        int speed;
        vec2 pos;
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
        world( std::string file = "default.tmx", std::string ordner = "worlds/");
        virtual ~world();

        float getCollisionX( fvec2 position, fvec2 change, fvec2 velocity, bool left = false);
        float getCollisionY( fvec2 position, fvec2 change, fvec2 velocity, bool up = false);
        tile *getCollisionTileY( fvec2 position, fvec2 change, fvec2 velocity);
        tile *getCollisionTileX( fvec2 position, fvec2 change, fvec2 velocity);
        int loadTypes( std::string file);
        bool load( std::string file, std::string ordner);
        tiletype *findType( int id);
        tile *getTile( tile *tilemap, vec2 pos);
        int getTypeIndex( int id, tiletype *type);
        void addBackground( tinyxml2::XMLElement* background, std::string ordner);
        void drawTile( graphic *graphic, int x, int y, tile *map);
        void drawBackAndForeground( graphic *graphic);
        void drawOverground( graphic *graphic);
        void process( graphic *graphic);
        void draw( graphic *graphic);
        void loadImageFiles( graphic *graphic);
        void setEndLevel( bool add_player ) { p_level_end = true; p_add_player = add_player;}
        bool leaveLevelasPlayer() { return p_add_player; }
        bool isLevelEnd() { return p_level_end; }
        float getGravity() { return p_gravity; }
        std::string getFileName() { return p_filename; }
        std::string getFile() { return p_file; }
        vec2 getTileSize() { return vec2( p_tilewidth, p_tilehight); }
        tile *getCollsionMap() { return p_tilemap_foreground; }
        vec2 getWorld() { return vec2(p_map_width, p_map_hight); }

        void setLoadWorld( std::string l_set) { p_needloadworld = l_set; }
        std::string needLoadWorld() { return p_needloadworld; }
        void setStartPoint( vec2 start) { p_startpoint = start; }
        vec2 getStartPoint() { return p_startpoint; }

        // get tile map
        tile *getOverlap() { return p_tilemap_overlap; }
        tile *getForeground() { return p_tilemap_foreground; }
        tile *getBackground() { return p_tilemap_background; }

        bool depthSort() { return p_depthSort; }
        std::string getMusicFile() { return p_music; }
    protected:

    private:
        std::string p_file;
        std::string p_filename;
        std::string loadParameter( tinyxml2::XMLNode *root, std::string name);

        tile *readTilemap( std::string tilemap);

        tile *p_tilemap_overlap;
        tile *p_tilemap_foreground;
        tile *p_tilemap_floorground;
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
        bool p_level_end;
        bool p_add_player;
        bool p_loadplayerdata;

        vec2 p_startpoint;

        std::vector<world_background> p_backgrounds;

        std::vector<tiletype> p_tiletypes;
        std::string p_needloadworld;
        std::string p_music;

        bool p_depthSort;

};

#endif // WORLD_H
