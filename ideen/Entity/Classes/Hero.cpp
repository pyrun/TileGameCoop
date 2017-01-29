#include "Hero.h"

void Hero( C1Entity *Obj ) {
    double MaxSpeed;
    Point Vertex = { Obj->pPosition.x +(Obj->pRect.x/2.0f), Obj->pPosition.y + (Obj->pRect.y/2.0f)}; // Vertex Wasser
    Point VertexUpLeft  = { Obj->pPosition.x + Obj->pRect.x * 0.3, Obj->pPosition.y }; // Vertex Kopf
    Point VertexUpRight = { Obj->pPosition.x + Obj->pRect.x * 0.7, Obj->pPosition.y }; // Vertex Kopf

    // Time
    Obj->Time++;
    // Größe vom Bild setzen
    Obj->SetRect( 24, 32);
    Obj->Animation.MaxStep = 4;
    Obj->Animation.Set( aSit, 4);

    // Rennen-Taste
    if( Obj->Control.Output.Run && Obj->Animation.Action != aSwim )
        MaxSpeed = 2.5;
    else
        MaxSpeed = 1.5;

    // Swim
    if( TargetWorld->CollisionLiquid(Vertex) && Obj->Animation.Action != aSwim) {
        Obj->Animation.Action = aSwim;
        Obj->Gravity.y /= 2;
    }
    if(Obj->Animation.Action == aSwim ) {
        if( Obj->Control.Output.Right ) {
            if( Obj->Animation.Step == 3 )
                Obj->Gravity.x += 0.1f;
            else
                Obj->Gravity.x += 0.09f;
            if(Obj->Gravity.x > 0)
                Obj->Animation.Dir = dRight;
        }
        if( Obj->Control.Output.Left ) {
            if( Obj->Animation.Step == 3 )
                Obj->Gravity.x -= 0.1f;
            else
                Obj->Gravity.x -= 0.09f;
            if(Obj->Gravity.x < 0)
                Obj->Animation.Dir = dLeft;
        }

        if( Obj->Control.Output.Jump ) {
            Obj->Gravity.y = -1;
            Obj->Jump = 1;
        }
        //if( Gravity.y > -1 && Jump)
        //    Jump = 0;

        if( !TargetWorld->CollisionLiquid(Vertex) ) {
            Obj->Animation.Action = aJump;
        }

        // Speed verlangsamenn
        Obj->Gravity.x /= 1.02;
        // Stärker verlangsamen falls man zu schnell ist
        if( abs(Obj->Gravity.y) >  MaxSpeed/2)
            Obj->Gravity.y /= 1.1;

        // Speed X Achse Grenzen einhalten
        if(abs(Obj->Gravity.x) > MaxSpeed) {
            if(Obj->Gravity.x > 0)
                Obj->Gravity.x = MaxSpeed;
            else
                Obj->Gravity.x = -MaxSpeed;
        }
    }

    // Walk
    if(Obj->Animation.Action == aIdle || Obj->Animation.Action == aWalk) {
        if(Obj->Control.Output.Right) {
            Obj->Gravity.x += 0.2;
            Obj->Animation.Action = aWalk;
            Obj->Speed = true;
            if(Obj->Gravity.x > 0)
                Obj->Animation.Dir = dRight;
        } else if( Obj->Control.Output.Left) {
            Obj->Gravity.x -= 0.2;
            Obj->Animation.Action = aWalk;
            Obj->Speed = true;
            if(Obj->Gravity.x < 0)
                Obj->Animation.Dir = dLeft;
        }
        if(abs(Obj->Gravity.x) > MaxSpeed) {
            if(Obj->Gravity.x > 0)
                Obj->Gravity.x = MaxSpeed;
            else
                Obj->Gravity.x = -MaxSpeed;
        }
    }
    //else if((Animation.Action == aIdle || Animation.Action == aWalk))
    //    Gravity.x /= 1.4;

    // Sit
    if( (Obj->Animation.Action == aWalk || Obj->Animation.Action == aIdle ) && Obj->Control.Output.Down )
        Obj->Animation.Action = aSit;
    if( Obj->Animation.Action == aSit ) {
        Obj->rCollision.p1.y = -Obj->pRect.y*0.30;
        //Obj->Speed = true;
        //if(Obj->Gravity.y == 0) // fals man auf den boden ist -> verlangsamen
            //Obj->Gravity.x /= 1.04;
            // ( Obj->Animation.Action == aSit && TargetWorld->GetTileCollision( VertexUpLeft) == 0 && TargetWorld->GetTileCollision( VertexUpRight) == 0)
        //
        if( !Obj->Control.Output.Down && TargetWorld->GetTileCollision( VertexUpLeft) == 0 && TargetWorld->GetTileCollision( VertexUpRight) == 0 ) {
            Obj->Animation.Action = aIdle;
            Obj->rCollision.p1.y = 0;
        }
        // Richtung ändern
        if( Obj->Control.Output.Right && Obj->Gravity.x >= 0 && Obj->Gravity.y == 0) {
            Obj->Animation.Dir = dRight; // Richtung nach Rechts
            Obj->Gravity.x += 0.1;
        } else if( Obj->Control.Output.Left && Obj->Gravity.x <= 0 && Obj->Gravity.y == 0 ) {
            Obj->Animation.Dir = dLeft; // Richtung nach Links
            Obj->Gravity.x -= 0.1;
        }
    }

    // Idle
    // Bei keiner bewegung automatisch zu Idle wechseln
    if( abs(Obj->Gravity.x) < 0.01 && Obj->Animation.Action == aWalk)
        Obj->Animation.Action = aIdle;

    // Jump
    if( Obj->Control.Output.Jump ) {
        if( Obj->Animation.Action == aWalk || Obj->Animation.Action == aIdle ||
           Obj->Animation.Action == aSit && (TargetWorld->GetTileCollision( VertexUpLeft) == 0 && TargetWorld->GetTileCollision( VertexUpRight) == 0 )) {// Bei Lauf oder Leerlauf
            // Sprungkraft
            Obj->Gravity.y = - (3.7f+Max(abs(Obj->Gravity.x), 1));
            Obj->Animation.Action = aJump;
            Obj->Animation.Step = 0;
            Obj->Jump = 1;
            Obj->rCollision.p1.y = 0; // Höhe rücksetzten

        } else if( Obj->Animation.Action == aJump && Obj->Jump == 1 && Obj->Animation.Step == 3 ) {
            if(Obj->Gravity.y > -3.0f)
                Obj->Gravity.y = -3.0f;
            Obj->Jump = 0;
        } /*else if( abs(Obj->Gravity.x) < 0.01 && Obj->Animation.Action == aSit && (TargetWorld->GetTileCollision( VertexUpLeft) != 0 || TargetWorld->GetTileCollision( VertexUpRight) != 0) ) { // Sitz und versucht zu springen
            if( Obj->Animation.Dir == dRight )
                Obj->Gravity.x += 0.1;
            else
                Obj->Gravity.x -= 0.1;
        }*/
    }
    // Air Jump right/left
    if(Obj->Animation.Action == aJump) {
        if( Obj->Control.Output.Right && Obj->Gravity.x < 1 )
            Obj->Gravity.x += 0.1;
        if( Obj->Control.Output.Left && Obj->Gravity.x > -1 )
            Obj->Gravity.x -= 0.1;
        // Anzeige Richtung ändern
        if( Obj->Gravity.x > 0 )
            Obj->Animation.Dir = dRight;
        else if( Obj->Gravity.x < 0 )
            Obj->Animation.Dir = dLeft;
    }
    // Animation
    // Animation Jump bei Gravity
    if(Obj->Gravity.y && (Obj->Animation.Action == aWalk || Obj->Animation.Action == aIdle))
        Obj->Animation.Action = aJump;
    if(Obj->Animation.Action == aJump && Obj->Gravity.y == 0)
        Obj->Animation.Action = aIdle;
    // Jump
    if(Obj->Animation.Action == aJump && Obj->Time%10 == 1 && Obj->Animation.Step != 3)
        Obj->Animation.CalcStep(); // Animation
    // Walk
    if(Obj->Animation.Action == aWalk && (Obj->Time%10 == 1 || (Obj->Control.Output.Run && Obj->Time%3 == 1 )))
        // Lauf Animation
        Obj->Animation.CalcStep();
    // Sit
    if(Obj->Animation.Action == aSit && Obj->Time%10 == 1 && (Obj->Control.Output.Right || Obj->Control.Output.Left) )
        // Lauf Animation
        Obj->Animation.CalcStep();
    // Swim
    if(Obj->Animation.Action == aSwim && Obj->Time%10 == 1)
        // Lauf Animation
        Obj->Animation.CalcStep();
    // Swim
    if(Obj->Animation.Action != aSwim)
        TargetWorld->AddGravity(Obj->Gravity);
    else
        Obj->Gravity.y += 0.04;
    Obj->CalcGravity();
}

/*

void C1Entity::Spikeball() {
    SetRect( 32, 32);

    // Animation
    Animation.MaxStep = 1;

}

void C1Entity::Flower() {
    SetRect( 16, 16);

    if( Data == 0) {
        Data = 1;

        Animation.Set( aDance, Data-1);
        Animation.Set( aDeath, 3);
        Animation.Action = aDance;
        Animation.MaxStep = 3;
    }

    // Schub Blume
    if( Data == 1 ) {
        Target = TargetEntity->CollisionObject( Number, dAll, 0.1f);
        if( Target+1 ) {
            TargetEntity->Get(Target)->Gravity.y = -7.0f;
        }
    }

    // PowerUP
    if( Data == 3) {
        Target = TargetEntity->CollisionObject( Number, dUp, 0.1f);
        if( Target+1 ) {
            Animation.Action = aDeath;
            Animation.MaxStep = 1;
        }
    }

    PowerUp();
}

void C1Entity::Frogti() {
    // Größe setzten
    SetRect( 16, 32);
    SpeedMax = 1;
    Animation.MaxStep = 4;
    EnemyWalk();
    // Animation action Index setzen
    Animation.Set( aWalk, 0);
    Animation.Set( aJump, 1);
}

void C1Entity::Coin() {
    // Größe setzten
    SetRect( 16, 16);
    // Step
    Animation.Step = (int)pPosition.x%5;
} */

/** ------------------------------------------------- Besonderes ------------------------------------------------ */
