#include "World.h"

int World_Version  = 1;

C1World *TargetWorld;


C1World::C1World() {
    TileSet = Graphic.LoadTexture( "Tilemap.bmp");
    /* Box2D */
    Gravity.x = 0.0f;
    Gravity.y = 0.18f;
}

C1World::~C1World() {
    Delete();
}

void C1World::Delete() {
    delete World;
    delete WorldBack;
}

void C1World::CreateWorld(int iWidth, int iHeight) {
    Width = iWidth;
    Height = iHeight;

    /* Erstelle den Speicher */
    World = new C1Tile[Width * Height];
    for( int X = 0; X < Width; X ++)
        for( int Y = 0; Y < Height; Y++) {
            SetTile( X, Y, -1, 0, World);
        }
    WorldBack = new C1Tile[Width * Height];
    for( int X = 0; X < Width; X ++)
        for( int Y = 0; Y < Height; Y++) {
            SetTile( X, Y, -1, 0, WorldBack);
        }

    /* Camera Set */
    Graphic.Camera->RectMax.x = Width*16;
    Graphic.Camera->RectMax.y = Height*16;

    Log.debug( 0, "Welt wird Erstellt %d %d", Width*16, Height);
}

void C1World::Load(char *File) {
    int X,Y, Version;
    FILE *fSave;
    C1Tile wSize;

    fSave = fopen(File,"rb");
    if(fSave == NULL) {
        Log.error( 0, "Konnte die Welt nicht laden! %s", File);
        return;
    }

    /* Anfag der Datei */
    fseek(fSave,0,SEEK_SET);

    /* Version check */
    fread(&Version, sizeof(int), 1, fSave);
    if(World_Version != Version) {
        Log.warning( 0, "Alte Datei! Kann nicht geladen werden! Weltversion %d", Version);
        fclose(fSave);
        return;
    }

    /* Karte löschen */
    Delete();

    /* Größe auslesen */
    fread (&Height, sizeof(int), 1, fSave);
    fread (&Width, sizeof(int), 1, fSave);

    /* Neue erstellen */
    CreateWorld( Height, Width);

    /* Map lesen */
        for(X = 0; X < Width; X++)
            for(Y = 0; Y < Height; Y++) {
                    //fprintf(fSave,"%d|%d|\n", World[X][Y][Z].Tile, World[X][Y][Z].Collision);
                    fread  (&World[ X*Height+Y ], sizeof (wSize), 1, fSave);
        }

    fclose(fSave);

    /* Nachricht ausgeben */
    Log.debug(0, "Welt wurde erfolgreich geladen. H\x94he %d Breite %d", Height, Width);
}

void C1World::SetWorldData(std::string Data, C1Tile *pWorld, int TileSetWidth, int iWidth, int iHeight) {
    int i, X = 0, Y = 0, Tile;
    int TileX, TileY;
    char *cTile;

    char cData[ Data.size()+1];
    strncpy( cData, Data.c_str(), sizeof( cData ));

    cTile = strtok(cData, ",");

    for( i = 0; cTile != NULL; i++) {
        TileY = 0;

        Tile = atoi(cTile);

        TileX = Tile-1;
        while( TileX >= TileSetWidth) {
            TileX -= TileSetWidth;
            TileY++;
        }

        SetTile( X, Y, TileX, TileY, pWorld);

        X++;
        if(X >= iWidth) {
            Y++;
            X = 0;
        }

        cTile = strtok(NULL, ",");
    }
}


void C1World::LoadTMX(char *File) {
    std::string Data;
    std::string Data2;
    tinyxml2::XMLDocument doc;
    tinyxml2::XMLElement *pRoot, *pParm;
    bool Load;
    /* Datei laden */
    Load = doc.LoadFile(File);
    if(Load) {
        Log.error( 0, "Konnte Datei nicht laden %s", File);
    }
    /* Root laden  */
    pRoot = doc.FirstChildElement("map");
    /* Höhe und Breite auslesen */
    int iWidth = atoi(doc.FirstChildElement( "map" )->Attribute( "width" ));
    int iHeight = atoi(doc.FirstChildElement( "map" )->Attribute( "height" ));
    /* Welt anpassen */
    CreateWorld( iWidth, iHeight);
    /* Breite vom Tileset-Bild */
    int TileSetWidth = (atoi(doc.FirstChildElement( "map" )->FirstChildElement( "tileset" )->FirstChildElement( "image" )->Attribute("width"))+8)/17;
    //
    pParm = pRoot->FirstChildElement("layer");
    Data = pParm->FirstChildElement( "data" )->GetText();
    pParm = pParm->NextSiblingElement("layer");
    if( pParm ) {
        // Data
        Data2 = pParm->FirstChildElement( "data" )->GetText();
    }
    else Data2 = Data;
    // Background
    std::string sBackground = "World/";
    if( pRoot->FirstChildElement( "imagelayer" )->FirstChildElement( "image" ) )
        sBackground.append( pRoot->FirstChildElement( "imagelayer" )->FirstChildElement( "image" )->Attribute( "source" ) );
    Background = Graphic.LoadTexture( sBackground );
    /* Kartendaten setzten */
    SetWorldData( Data, WorldBack,  TileSetWidth, iWidth, iHeight);
    SetWorldData( Data2, World,     TileSetWidth, iWidth, iHeight);
}


void C1World::Save(char *File) {
    int X,Y;
    FILE *fSave;
    C1Tile wSize;

    fSave = fopen(File,"wb");
    if(fSave == NULL) {
        Log.error( 0, "Konnte die Welt nicht speichern! Welt %s", File);
        return;
    }

    /* Anfag der Datei */
    fseek(fSave,0,SEEK_SET);

    /* Version speichern */
    fwrite (&World_Version, sizeof(int), 1, fSave);

    /* Size speichern */
    fwrite (&Height, sizeof(int), 1, fSave);
    fwrite (&Width, sizeof(int), 1, fSave);

    /* Layerweiße abspeichern */
    for(X = 0; X < Width; X++)
        for(Y = 0; Y < Height; Y++) {
                fwrite (&World[ X*Height+Y ], sizeof (wSize), 1, fSave);

    }

    /* Schließen */
    fclose(fSave);
    // Notiz das die Welt gespeichert wurde
    Log.debug( 0, "Welt gespeichert");
}

int C1World::GetTileX(int X, int Y) {
    return GetTileX( X, Y, World);
}
int C1World::GetTileX(int X, int Y, C1Tile *pWorld) {
    if( X < 0 || X >= Width)
        return 0;
    if( Y >= Height )
        return -1;
    if( Y < 0)
        return -1;
    // Abfrage der Tile
    return pWorld[ X*Height+Y ].TileX;
}

int C1World::GetTileY(int X, int Y) {
    // Abfragen der Tile Infomation
    return GetTileY( X, Y, World);
}
int C1World::GetTileY(int X, int Y, C1Tile *pWorld) {
    if( X < 0 || Y < 0 || X >= Width || Y >= Height)
        return -1;
    // Abfragen der Tile Infomation
    return pWorld[ X*Height+Y ].TileY;
}

void C1World::SetTile(int X, int Y, int TileX, int TileY, C1Tile *pWorld) {
    // Damit es bei einem Fehler nicht das Spiel abstürtzt
    if( X < 0 || Y < 0 || X >= Width || Y >= Height) {
        Log.error( 0, "SetTile: Tile %d %d ist auser dem Berech!");
        return;
    }
    // Falsche Kodierung von Tile/TileY
    if( TileX < -1 || TileY < 0)
        return;
    // Unsichbarer Tile
    if( TileX == -1 ) {
        TileY = 0;
    }
    // Ändern der Daten
    pWorld[ X*Height+Y ].TileX = TileX;
    pWorld[ X*Height+Y ].TileY = TileY;
}

void C1World::DrawTile(int X, int Y, int TileX, int TileY) {
    X = X*16;
    Y = Y*16;
    // Malen der Sprites
    Graphic.DrawSprite( TileSet, X, Y, 16, 16, TileX, TileY, dRight, 1);
}

/** \brief
 *
 * \param Schwerkraft
 * \param Punkt der Gecheckt wird
 * \param Derzeitige Position
 * \param Collisions Rect; Hülle vom Object
 * \return Neue Position
 *
 */
double C1World::CollisionSlope(Vector &Gravity, Point Vertex, Point Position, Point Rect) {
    return CollisionSlope( Gravity, Vertex, Position, Rect, false);
}
double C1World::CollisionSlope(Vector &Gravity, Point Vertex, Point Position, Point Rect, bool noFix) {
    int TileX = Vertex.x/16;
    int TileY = (Vertex.y-8)/16;
    double C = 0;
    // Darunter ene schräge gefunden -> die nützten
    if(GetTileCollisionInt( TileX, TileY+1) == 2) {
        TileY += 1;
    }
    // Keine Schräge auf dem Vertex -> beenden
    if(GetTileCollisionInt( TileX, TileY) != 2)
        return 0;
    // Schrägeart berechnen
    switch( GetTileX( TileX, TileY)%6 ) {
    case 1: /* Schräg 45° 1#  rechts */
        //C = TileY*16 - ( 16 - ( TileX*16 - Vertex.x +1)) + (32-Rect.y);
        C = TileY*16 - ( 16 - (TileX*16+2 - (Vertex.x+1))) + (32-Rect.y);
    break;
    case 0: /* Schräg 45° 1#  links */
        //C = TileY*16 -32 - (  (TileX*16) - (Vertex.x + 1) ) + (32-Rect.y);
        C = TileY*16 -32 - ( ( ( (TileX*16) - (Vertex.x+1)) )) + (32-Rect.y);
    break;
    case 2: /* Schräg 22,5° 1#  links */
        C = TileY*16 -32 - ( ( ( (TileX*16) - (Vertex.x+1)) ) / 2) + (32-Rect.y);
    break;
    case 3: /* Schräg 22,5° 1#  rechts */
        C = TileY*16-16 - (( 16 - (TileX*16+2 - (Vertex.x+1)) )/2) + (32-Rect.y);
    break;
    case 4: /* Schräg 22,5° 2#  links */
        C = TileY*16 -24 - ( ( ( (TileX*16) - (Vertex.x+2)) ) / 2) + (32-Rect.y);
    break;
    case 5: /* Schräg 22,5° 2# rechts */
        C = TileY*16-8 - (( 16 - (TileX*16+2 - (Vertex.x+1)) )/2) + (32-Rect.y);
    break;
    default:
    break;
    }
    // Ausnahmeregel -> Fall das Objekt Drüber ist oder weg schwebt(Jump?)
    if( abs(C-(Position.y+Gravity.y)) > 1+abs(Gravity.x)+Gravity.y )
            C = 0;
    else if( C && noFix == false ) { // Gravity zurücksetzten da es den Boden berührt
        //Gravity.x += Gravity.y;
        Gravity.y = 0;
    }
    return C; // Rückgabe
}

bool C1World::CollisionLiquid(Point Vertex) {
    int TileX = (int)Vertex.x/16;
    int TileY = (int)Vertex.y/16;
    // Was Wasser ist
    if( GetTileX(TileX, TileY, WorldBack) == 0 && GetTileY(TileX, TileY, WorldBack) > 0)
        return true;
    return false;
}


double C1World::CollisionY(Vector &Gravity, Point Vertex) {
    if(GetTileCollision(Vertex) == 0 || ( GetTileCollision(Vertex) == 3 && Gravity.y < 0 ) || GetTileCollision(Vertex) == 2 ) return 0;
    if(Vertex.y < 0)
        return 0;

    double C = Gravity.y-( Vertex.y -((int)Vertex.y/16*16)); // Neue Gravitation
    double TileC = ( Vertex.y -((int)Vertex.y/16*16)); // Unterschied zwischen Block Oben und Person-Vertex

    // Für Einweg gänge =)
    if( TileC > 2+abs(Gravity.y)+abs(Gravity.x) && GetTileCollision(Vertex) == 3 )
        return 0;

    // Nicht Zu Stark Nach oben
    /*if( C*10 < Gravity.y) {
        return 0;
    }*/

    Gravity.y = 0;

    return C;
}

int C1World::GetTileCollision(Point Vertex) {
    if( Vertex.x < 0 )
        return 1;
    if( Vertex.y < 0 )
        return 0;
    // Gibt Collsion mask ab
    return GetTileCollisionInt(Vertex.x/16, Vertex.y/16);
}

int C1World::GetTileCollisionInt( int X, int Y) {
    if( GetTileY( X, Y) ==  1 ) // Nur von unten passierbar
        return 3;
    if( GetTileX( X, Y) == -1 || // Luft
        (GetTileX( X, Y) == 0 && GetTileY( X, Y) > 0) ) // Wasser
        return 0;
    if( GetTileY( X, Y) ==  0 ) // Schräge
        return 2;
    return 1;
}

void C1World::AddGravity( Vector &V) {
    V = V + Gravity;
}

void C1World::Draw() {
    int X, Y;
    Point Cam = Graphic.Camera->GetView().p1;
    Point CamRect = Graphic.Camera->GetView().p2;
    // Background zeichnen
    Graphic.Draw( Background, Cam.x, Cam.y, CamRect.x, CamRect.y, 0, (int)Cam.x/2, (int)Cam.y, dRight, 0, 0);
    // Zeichne die Sichbaren Tiles
    for( X = Cam.x/16; X < (Cam.x+CamRect.x)/16; X ++) {
        for( Y = Cam.y/16; Y < (Cam.y+CamRect.y)/16; Y++) {
            if( WorldBack[ X*Height+Y].TileX != -1)
                DrawTile( X, Y, WorldBack[ X*Height+Y].TileX, WorldBack[ X*Height+Y ].TileY);
            if( World[ X*Height+Y].TileX != -1)
                DrawTile( X, Y, World[ X*Height+Y].TileX, World[ X*Height+Y ].TileY);
        }
    }
}
