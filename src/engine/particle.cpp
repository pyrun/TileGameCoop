#include "particle.h"

particle::particle( int number)
{
    //null
    p_type = par_none;
    p_id = number;
    p_deadtime.start();
}

particle::~particle()
{
    //dtor
}

#include <stdio.h>
#include <math.h>

void particle::draw( graphic *graphic, font *font) {
    int l_max = p_lifetime;
    int l_now = p_lifetime-p_deadtime.getTicks();
    static float l_e = 2.71;
    fvec2 l_pos = this->p_pos;

    // calc alpha - fade out
    float l_alpha = 8.0f/l_max*l_now; // calc C
    l_alpha = 1.0f-pow( l_e, -l_alpha); // 1.0f-pow( eule, -Zeitkonstante ); https://de.wikipedia.org/wiki/Zeitkonstante
    l_alpha *= 255.f;
    if( l_alpha < 0)
        l_alpha = 0;

    if( p_hud == true)
        l_pos = this->p_pos + graphic->getCamera();

    // draw type
    switch( this->p_type) {
        case par_text: font->drawMessage( graphic, this->p_data, l_pos.tovec2(), this->p_zoom.x, (int)l_alpha, false, false); break;
        case par_pic: graphic->drawImage( this->p_img, l_pos.tovec2(), vec2(), vec2()); break;
        default: break;
    }
}

particle_list::particle_list() {
     p_number = 0;
}

particle_list::~particle_list() {

}

int particle_list::createParticel( particle_type type, fvec2 pos, fvec2 velocity, int lifetime, std::string data, fvec2 zoom, bool asHUD) {
    particle l_par( p_number);

    // Daten setzten
    l_par.setType( type);
    l_par.setPosition( pos);
    l_par.setData( data);
    l_par.setVelocity( velocity);
    l_par.setLifeTime( lifetime);
    l_par.setZoom( zoom);
    l_par.setAsHUD( asHUD);

    // eindeutige ID erhöhen
    p_number++;

    // hinzufügen
    p_particels.push_back( l_par);
    return p_number;
}

void particle_list::draw( graphic *graphic, font *font) {
    for( int i = 0; i < (int)p_particels.size(); i++) {
        particle *l_par = &p_particels[i];

        // call draw
        l_par->draw( graphic, font);

        if( l_par->isDead()) {
            p_particels.erase( p_particels.begin()+i);
            i--;
        }
    }
}

particle *particle_list::getParticel( int id) {
    particle *l_par = NULL;
    for( int i = 0; i < (int)p_particels.size(); i++) {
        if( p_particels[i].getId() == id)
            l_par = &p_particels[i];
    }
    return l_par;
}
