#ifndef SAVE_H
#define SAVE_H

#include "../xml/tinyxml2.h"
#include "types.h"

#include "entity.h"

#define SAVE_FILE savefile.xml

class save
{
    public:
        save();
        virtual ~save();

        void load( entitylist *entity);
        void save_once( entitylist *entity);
    protected:

    private:
};

#endif // SAVE_H
