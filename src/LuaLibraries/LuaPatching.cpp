#include "LuaPatching.hpp"

#include "../../include/VM/lua.h"
#include "../../include/VM/lualib.h"

#include <SDL3/SDL_stdinc.h>


int Game::Lua::CLibraries::Patching::PrefixPatch(lua_State* State) {
    luaL_checktype(State, 1, LUA_TFUNCTION);
    luaL_checktype(State, 2, LUA_TFUNCTION);

    if (lua_getmetatable(State, 1) == 0) {
        lua_createtable(State, 0, 3);

        lua_setfield(State, -2, "__call");

        lua_createtable(State, 0, 1);

        lua_pushinteger(State, 1);
        lua_pushvalue(State, 2);
        lua_settable(State, -3);

        lua_setfield(State, -2, "_PrefixPatches");

        lua_newtable(State);
        lua_setfield(State, -2, "_SuffixPatches");

        lua_setmetatable(State, 1);
    }

    return 0;
}