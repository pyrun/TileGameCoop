#ifndef LUA_LOADER_H
#define LUA_LOADER_H

#include <string>
#include <vector>

// debian
#ifdef IMADEBIAN
#include <lua5.3/lua.hpp>
#else
#include <lua.hpp>
#endif // IMADEBIAN

class lua_loader {
    public:
        lua_loader();
        virtual ~lua_loader();
        std::string name;
        void (*func)( lua_State *);
};

extern std::vector<lua_loader *> p_lua_loader;
extern void lua_loader_add( std::string name, void (*func)( lua_State *));

#endif // LUA_LOADER_H
