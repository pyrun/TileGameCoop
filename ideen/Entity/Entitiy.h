#include "../World/World.h"
#include "../Game/Vector.h"
#include "../Game/Graphic.h"
#include "../Game/Control.h"
#include "../System/Log.h"

#ifndef ENTITY_H
#define ENTITY_H 1

enum eType { eHero, eTurtle, eCoin, eSnail, eFrogti};

eType GetType(std::string Name);

class C1Collision {
private:
public:
    int XCol[16];
    int YCol[16];
};

class C1Entity {
public:
    char Live;
    eType Type;

    int Time;
    int Jump;
    float SpeedMax;

    int Texture;

    Point Position;
    Point Rect;

    Vector Gravity;

    C1Animation Animation;
    C1Control Control;

    bool IsFree;
public:
    C1Entity();
    ~C1Entity();

    void SetPosition(float X, float Y);

    void Process();
    void Draw();

    bool CollisionX(Point Vertex);
    bool CollisionY(Point Vertex);

    void CalcGravity();

    /* Vordefinierte typen */
    void EnemyWalk(); // Bodeneinheit

    /* Einzelne Einheiten */
    void Frogti();
    void Snail();
    void Coin();
    void Turtle();
    void Hero();

    void SetRect(int W, int H);
};

#endif // ENTITY_H
