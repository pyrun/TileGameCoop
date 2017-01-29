#include "Entitiy.h"

eType GetType(std::string Name) {
    if("Hero" == Name) return eHero;
    if("Turtle" == Name) return eTurtle;
    if("Coin" == Name) return eCoin;
    if("Snail" == Name) return eSnail;
    if("Frogti" == Name) return eFrogti;

    return eHero;
}

C1Entity::C1Entity() {
    Texture = -1;

    Position.x = 0;
    Position.y = 0;

    Rect.x = 0;
    Rect.y = 0;

    Gravity.x = 0;
    Gravity.y = 0;

    Time = 0;
    SpeedMax = 1;

    Animation.Action = aIdle;
    Animation.Dir = dLeft;

    Control.Reset();
}

C1Entity::~C1Entity() {
    IsFree = 1;
}

void C1Entity::SetRect(int W, int H) {
    if(Rect.x)
        return;

    /* Größe vom Bild setzen */
    if(W && H) {
        Rect.x = W;
        Rect.y = H;
        return;
    } else if(Texture != -1) {
        Rect.x = 16;//Graphic.GetWidth( Texture );
        Rect.y = 16;//Graphic.GetHeight( Texture );
        return;
    } else return;
    /* Wieder auf NULL setzen*/
    Rect.x = 0;
    Rect.y = 0;
}

bool C1Entity::CollisionX(Point Vertex) {
    /* X */
    if( Gravity.x != 0)
        if( TargetWorld->GetTileCollision( Vertex) == 1) {
            return 1;
        }
    return 0;
}

bool C1Entity::CollisionY(Point Vertex) {
    /* Rechts */
    if( Gravity.y != 0 )
        if( TargetWorld->GetTileCollision( Vertex )) {
            return 1;
        }
    return 0;
}

void C1Entity::CalcGravity() {
    Point TempPos;
    float C;

    float OffsetXDown = 0.1f;
    float OffsetYUp = 0.15f;

    Point VertexDown1 = { Position.x +(Rect.x/2.0f) - (Rect.x*OffsetXDown) + Gravity.x, Position.y + Rect.y         + Gravity.y};
    Point VertexDown2 = { Position.x +(Rect.x/2.0f) + (Rect.x*OffsetXDown) + Gravity.x, Position.y + Rect.y         + Gravity.y};

    Point VertexUp1 = { Position.x +(Rect.x/2.0f) - (Rect.x*OffsetXDown) + Gravity.x, Position.y + (Rect.y*OffsetYUp) + Gravity.y};
    Point VertexUp2 = { Position.x +(Rect.x/2.0f) + (Rect.x*OffsetXDown) + Gravity.x, Position.y + (Rect.y*OffsetYUp) + Gravity.y};

    Point VertexRight1 = { Position.x +(Rect.x/2.0f) + (Rect.x*(OffsetXDown+0.1f)) + Gravity.x, Position.y + (Rect.y*0.35f) + Gravity.y};
    Point VertexRight2 = { Position.x +(Rect.x/2.0f) + (Rect.x*(OffsetXDown+0.1f)) + Gravity.x, Position.y + (Rect.y*0.69f) + Gravity.y};

    Point VertexLeft1 = { Position.x +(Rect.x/2.0f) - (Rect.x*(OffsetXDown+0.1f)) + Gravity.x, Position.y + (Rect.y*0.35f) + Gravity.y};
    Point VertexLeft2 = { Position.x +(Rect.x/2.0f) - (Rect.x*(OffsetXDown+0.1f)) + Gravity.x, Position.y + (Rect.y*0.69f) + Gravity.y};

    /* Fix */
    if( Rect.y < 32) {
        VertexLeft2.y -= (Rect.y*0.20f);
        VertexRight2.y -= (Rect.y*0.20f);
    }

    Point Vertex9 = { Position.x + (Rect.x/2.0f) + Gravity.x, Position.y + Rect.y + Gravity.y};

    /** Oben */
    if( Gravity.y < 0 ) {
        // Collision Vertex 1
        Position.y += TargetWorld->CollisionY(Gravity , VertexUp1);
        // Falls Vertex davor nicht anstöst
        if( Gravity.y < 0 )
            Position.y += TargetWorld->CollisionY(Gravity , VertexUp2);
        // Collsion -> Etwas anpassen
        if(Gravity.y == 0) {
            Position.y += 16;
            Gravity.y = +0.1;
        }
    }

    /** Boden */
    if( Gravity.y > -0.2f ) {

        /* Entweder Kollision mit Schräge oder mit den Soliden tiles */
        if(TargetWorld->CollisionSlope(Gravity, Vertex9, Position, Rect, 1) != 0) {
            Position.y =  TargetWorld->CollisionSlope(Gravity, Vertex9, Position, Rect);
        }
        if( Gravity.y > 0 ) {
            Position.y += TargetWorld->CollisionY(Gravity , VertexDown1);
            if( Gravity.y > 0)
                Position.y += TargetWorld->CollisionY(Gravity , VertexDown2);
        }

    }

    /** Rechts */
    if( CollisionX(VertexRight1) && Gravity.x > 0 ) {
        Gravity.x = 0;
    }
    if( CollisionX(VertexRight2) && Gravity.x > 0 ) {
        Gravity.x = 0;
    }

    /** Left */
    if( CollisionX(VertexLeft1) && Gravity.x < 0 ) {
        Gravity.x = 0;
    }
    if( CollisionX(VertexLeft2) && Gravity.x < 0 ) {
        Gravity.x = 0;
    }

    /*if( CollisionX(VertexRight2) ) {
        C = Gravity.x-( VertexRight2.x -((int)VertexRight2.x/16*16));
        if( (int)(C*10) > 3)
            Position.x += C;
        if(Gravity.x > 0)
            Gravity.x = 0;
    }*/

    /** Links  */

    /*if( CollisionX(Vertex3) ) {
        float C = ( Vertex3.x -((int)Vertex3.x/16*16)) - Gravity.x -16;
        if( (int)(C*10) > 10)
            Position.x -= C;
        if(Gravity.x < 0)
            Gravity.x = 0;
    }*/

    TempPos = Position.AddVector(Gravity);
    Position = TempPos;
}

/* Type */
/* Enemy Walk */
void C1Entity::EnemyWalk() {
    Time++;

    // Collision
    Point VertexRight = { Position.x +(Rect.x/1.0f) + (Rect.x*(1)) + Gravity.x, Position.y + Gravity.y};
    Point VertexLeft  = { Position.x +(Rect.x/1.0f) - (Rect.x*(1)) + Gravity.x, Position.y + Gravity.y};

    /* Nicht stehen bleiben */
    if( Animation.Action == aIdle) {
        Animation.Action = aWalk;
        Gravity.x -= 0.1;
    }

    /** Walk */
    if( Animation.Action == aWalk) {
        if( Gravity.x < 0)
            Animation.Dir = dLeft;
        else if( Gravity.x > 0)
            Animation.Dir = dRight;

        if( CollisionX(VertexRight) && -SpeedMax < Gravity.x)
            Gravity.x -= 0.1f;
        else if( CollisionX(VertexLeft) && SpeedMax > Gravity.x)
            Gravity.x += 0.1f;
        // Speed erhöhen
        else if( Gravity.x > 0 && SpeedMax > Gravity.x)
            Gravity.x += 0.1f;
        else if( Gravity.x < 0 && -SpeedMax < Gravity.x)
            Gravity.x -= 0.1f;
    }

    /** Animation */
    /* Jump */
    if(Animation.Action == aJump && Time%10 == 1) {
        /* Lauf Animation */
        Animation.Step = (Animation.Step+1)%3;
    }
    /* Walk */
    if(Animation.Action == aWalk && Time%10 == 1) {
        /* Lauf Animation */
        Animation.Step = (Animation.Step+1)%3;
    }
    /* Idle */
    if(Animation.Action == aIdle && Time%10 == 1) {
        Animation.Step = (Animation.Step+1)%3;
    }

    /* Gravity */
    TargetWorld->AddGravity(Gravity);
    CalcGravity();
}

void C1Entity::Frogti() {
    // Größe setzten
    SetRect( 16, 32);

    SpeedMax = 1;

    EnemyWalk();

    // Animation action Index setzen
    Animation.Set( aWalk, 0);
    Animation.Set( aJump, 0);
}

void C1Entity::Snail() {
    // Größe setzten
    SetRect( 28, 18);

    SpeedMax = 0.2;

    EnemyWalk();

    // Animation action Index setzen
    Animation.Set( aWalk, 0);
}

void C1Entity::Coin() {
    // Größe setzten
    SetRect( 16, 16);

    // Step
    Animation.Step = (int)Position.x%5;
}

void C1Entity::Turtle() {
    /* Größe vom Bild setzen */
    SetRect( 32, 16);

    SpeedMax = 0.1;

    EnemyWalk();

    /* Gravity*/
    TargetWorld->AddGravity(Gravity);
    CalcGravity();
}

/** ------------------------------------------------- Besonderes ------------------------------------------------ */
void C1Entity::Hero() {
    float MaxSpeed;

    Point Vertex = { Position.x +(Rect.x/2.0f), Position.y + (Rect.y/2.0f)};

    Time++;

    /* Größe vom Bild setzen */
    SetRect( 32, 32);

    /* Rennen */
    if( Control.Run && Animation.Action != aSwim )
        MaxSpeed = 2.0;
    else
        MaxSpeed = 1.5;

    /** Swim */
    if( TargetWorld->CollisionLiquid(Vertex) && Animation.Action != aSwim) {
        Animation.Action = aSwim;
        Gravity.y /= 2;
    }
    if(Animation.Action == aSwim ) {

        if( Control.Right ) {
            if( Animation.Step == 3 )
                Gravity.x += 0.1f;
            else
                Gravity.x += 0.09f;
            if(Gravity.x > 0)
                Animation.Dir = dRight;
        }
        if( Control.Left ) {
            if( Animation.Step == 3 )
                Gravity.x -= 0.1f;
            else
                Gravity.x -= 0.09f;
            if(Gravity.x < 0)
                Animation.Dir = dLeft;
        }

        if( Control.Jump && !Jump) {
            Gravity.y -= 3;
            Jump = 1;
        }
        if( Gravity.y > 0 && Jump)
            Jump = 0;

        /*if( Control.Down ) {
            if(Gravity.y < MaxSpeed/2) {
                if( Animation.Step == 3 )
                    Gravity.y += 0.1;
                else
                    Gravity.y += 0.09;
            }
        }
        if( Control.Up && Animation.Step == 3) {
            if(Gravity.y > -MaxSpeed/2) {
                if( Animation.Step == 3 )
                    Gravity.y -= 0.1;
                else
                    Gravity.y -= 0.09;
            }
        }*/

        if( !TargetWorld->CollisionLiquid(Vertex) ) {
            Animation.Action = aJump;
            Gravity.y = -4;
        }


        Gravity.x /= 1.01;

        if( abs(Gravity.y) >  MaxSpeed/2)
            Gravity.y /= 1.1;


        if(abs(Gravity.x) > MaxSpeed)
            if(Gravity.x > 0)
                Gravity.x = MaxSpeed;
            else
                Gravity.x = -MaxSpeed;
    }

    /** Walk */
    if((Animation.Action == aIdle || Animation.Action == aWalk) && Control.Right) {
        Gravity.x += 0.2;
        Animation.Action = aWalk;
        if(Gravity.x > 0)
            Animation.Dir = dRight;
    }
    else if((Animation.Action == aIdle || Animation.Action == aWalk) && Control.Left) {
        Gravity.x -= 0.2;
        Animation.Action = aWalk;
        if(Gravity.x < 0)
            Animation.Dir = dLeft;
    }
    else if((Animation.Action == aIdle || Animation.Action == aWalk))
        Gravity.x /= 1.4;
    if(Animation.Action == aWalk)
        if(abs(Gravity.x) > MaxSpeed)
            if(Gravity.x > 0)
                Gravity.x = MaxSpeed;
            else
                Gravity.x = -MaxSpeed;

    /** Idle */
    if( abs(Gravity.x) < 0.01 && Animation.Action == aWalk)
        Animation.Action = aIdle;
    /*if( Gravity.y == 0.00 && Animation.Action == aJump)
        Animation.Action = aIdle;*/

    /** Jump */
    if( Control.Jump && (Animation.Action == aWalk || Animation.Action == aIdle)) {
        Gravity.y -= 4,7;
        Animation.Action = aJump;
        Animation.Step = 0;
        Jump = 1;
    } else if( Control.Jump && Animation.Action == aJump && Jump == 1 && Animation.Step == 3) {
        Gravity.y = -3,5;
        Jump = 0;
    }

    /* Jump right/left */
    if(Animation.Action == aJump) {
        if(Control.Right && Gravity.x < 1)
            Gravity.x += 0.1;
        if(Control.Left && Gravity.x > -1)
            Gravity.x -= 0.1;
    }

    if(Gravity.y > 0.5 && (Animation.Action == aWalk || Animation.Action == aIdle))
        Animation.Action = aJump;

    if(Animation.Action == aJump && Gravity.y == 0)
        Animation.Action = aIdle;

    if(Animation.Action == aWalk && Gravity.x > 0 && Control.Left) {
        Gravity.x -= 0.8;
        Animation.Action = aWalk;
        Animation.Dir = dLeft;
    }

    /** Animation */
    /* Jump */
    if(Animation.Action == aJump && Time%10 == 1 && Animation.Step != 3) {
        Animation.Step = (Animation.Step+1)%4;
    }
    /* Walk */
    if(Animation.Action == aWalk && (Time%10 == 1 || (Control.Run && Time%3 == 1 ))) {
        /* Lauf Animation */
        Animation.Step = (Animation.Step+1)%4;
    }
    /* Swim */
    if(Animation.Action == aSwim && Time%10 == 1) {
        /* Lauf Animation */
        Animation.Step = (Animation.Step+1)%4;
    }

    if(Animation.Action != aSwim)
        TargetWorld->AddGravity(Gravity);
    else
        Gravity.y += 0.04;
    CalcGravity();
}

void C1Entity::SetPosition(float X, float Y) {
    Position.x = X;
    Position.y = Y;
}

void C1Entity::Process() {
    // Überlauf verhindern ( kommt alle paar wochen aber eine kleine Vorsorge für suchtis =3 )
    if(Time > 2000)
        Time -= 1000;

    if(Position.y > TargetWorld->Height*16) {
        IsFree = 1;
        return;
    }

    switch(Type) {
        case eHero: Hero(); break;
        case eTurtle: Turtle(); break;
        case eCoin: Coin(); break;
        case eSnail: Snail(); break;
        case eFrogti: Frogti(); break;
        default:
            //Log.debug( 0, "Kann nicht berechnet werden!");
        break;
    }
    /* Eingabe zurücksetzten */
    Control.Reset();
}

int gHero = -1;
int gTurtle = -1;
int gCoin = -1;
int gSnail = -1;
int gFrogti = -1;

void C1Entity::Draw() {
    /* Laden fallst nicht vorhanden */
    if(gHero == -1)
        gHero = Graphic.LoadTexture("Hero.png");
    if(gTurtle == -1)
        gTurtle = Graphic.LoadTexture("Turtle.png");
    if(gCoin == -1)
        gCoin = Graphic.LoadTexture("Coin.png");
    if(gSnail == -1)
        gSnail = Graphic.LoadTexture("Snail.png");
    if(gFrogti == -1)
        gFrogti = Graphic.LoadTexture("Frogti.png");
    /* Einzeln Malen */
    switch(Type) {
        case eHero: Texture = gHero; break;
        case eTurtle: Texture = gTurtle; break;
        case eCoin: Texture = gCoin; break;
        case eSnail: Texture = gSnail; break;
        case eFrogti: Texture = gFrogti; break;
        default: Log.debug( 0, "Kann nicht gemalt werden!"); break;
    }

    if(Texture != -1)
        Graphic.DrawSprite( Texture, Position.x, Position.y, Rect.x, Rect.y, Animation.Step, Animation.Get(Animation.Action), Animation.Dir );
}
