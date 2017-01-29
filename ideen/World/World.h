#include "../../lib/tinyxml2-master/tinyxml2.h"
#include <math.h>

#include "../Game/Graphic.h"
#include "../System/Extra.h"

#ifndef WORLD_H
#define WORLD_H 1

class C1Tile {
public:
    int TileX;
    int TileY;
};

class C1World {
public:
    C1Tile *World;
    C1Tile *WorldBack;
    // Bild von TileSet
    int TileSet;
    // Bild Backround
    int Background;
    Vector Gravity;
public:
    C1World();
    ~C1World();

    // Höhe & Breite
    int Width;
    int Height;

    void CreateWorld(int iWidth, int iHeight);
    void Delete();

    void SetTile(int X, int Y, int TileX, int TileY, C1Tile *pWorld);
    int GetTileX(int X, int Y); // X
    int GetTileX(int X, int Y, C1Tile *pWorld);
    int GetTileY(int X, int Y); // Y
    int GetTileY(int X, int Y, C1Tile *pWorld);


    double CollisionSlope(Vector &Gravity, Point Vertex, Point Position, Point Rect);
    double CollisionSlope(Vector &Gravity, Point Vertex, Point Position, Point Rect, bool noFix);

    bool CollisionLiquid(Point Vertex);

    double CollisionY(Vector &Gravity, Point Vertex);
    int CollisionDetectionX(int Xfrom, int Yfrom, int Yto);
    int GetTileCollision(Point Vertex);
    int GetTileCollisionInt( int X, int Y);

    void AddGravity( Vector &V);

    void SetWorldData(std::string Data, C1Tile *pWorld, int TileSetWidth, int iWidth, int iHeight);

    void Load(char *File);
    void LoadTMX(char *File);
    void Save(char *File);

    void Draw();
    void DrawTile(int X, int Y, int TileX, int TileY);
};

extern C1World *TargetWorld;

#endif // WORLD_H
