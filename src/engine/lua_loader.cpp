#include "lua_loader.h"

std::vector<lua_loader *> p_lua_loader;

lua_loader::lua_loader() {
}

lua_loader::~lua_loader() {
    //dtor
}

void lua_loader_add( std::string name, void (*func)( lua_State *)) {
    // install load lua at lua_loader
    bool l_loaded = false;
    for( auto const& l_loader:p_lua_loader)
        if( l_loader->name == name)
            l_loaded = true;
    if( !l_loaded) {
        lua_loader *l_loader_obj = new lua_loader;
        l_loader_obj->func = func;
        l_loader_obj->name = name;
        p_lua_loader.push_back( l_loader_obj);
    }
}
