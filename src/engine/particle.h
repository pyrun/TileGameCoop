#ifndef PARTICLE_H
#define PARTICLE_H

#include <vector>
#include <string>
#include "../graphic/graphic.h"
#include "../graphic/font.h"

enum particle_type{ par_none = 3, par_text, par_pic};

class particle
{
    public:
        particle( int number);
        virtual ~particle();
        void setType( particle_type type) { p_type = type; }
        void setPosition( fvec2 pos) { p_pos = pos; }
        void setVelocity( fvec2 vel) { p_velocity = vel; }
        void setData( std::string data) { p_data = data; }
        void setImg( image *image) { p_img = image; }
        void setZoom( fvec2 zoom) { p_zoom = zoom; }
        void setLifeTime( int time) { p_lifetime = time; }
        void setAsHUD( bool set) { p_hud = set; }

        int getId() { return p_id; }
        bool isDead() { return p_deadtime.getTicks() > p_lifetime?true:false; }

        void draw( graphic *graphic, font *font);
    protected:

    private:
        int p_id;
        particle_type p_type;
        fvec2 p_pos;
        fvec2 p_velocity;
        std::string p_data;
        image *p_img;
        fvec2 p_zoom;
        bool p_hud;

        int p_lifetime;
        timer p_deadtime;
};

class particle_list
{
    public:
        particle_list();
        virtual ~particle_list();

        int createParticel( particle_type type, fvec2 pos, fvec2 velocity, int lifetime, std::string data, fvec2 zoom = fvec2( 1, 1), bool asHUD = false);

        void draw( graphic *graphic, font *font);

        particle *getParticel( int id);

        void clear() { p_particels.clear(); }
    protected:

    private:
        std::vector<particle> p_particels;
        int p_number;
};

#endif // PARTICLE_H
