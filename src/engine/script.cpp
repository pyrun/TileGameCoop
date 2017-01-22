#include "script.h"


script::script()
{
    lua_State *state = luaL_newstate();
    lua_close(state);
    //ctor
}

script::~script()
{
    //dtor
}
