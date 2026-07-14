#pragma once

#include <lua-5.5.0/lua.hpp>

#include <SDL3/SDL_stdinc.h>


namespace Game::Lua::CLibraries::Patching {
	inline void Init(lua_State* State);

	static int PrefixPatch(lua_State* State);
}

constexpr static const char* _InvokePatches_LuaSource
= R"(return function()
)";

void Game::Lua::CLibraries::Patching::Init(lua_State* State) {

    if (lua_getglobal(State, "Utils") != LUA_TTABLE) {
        lua_pop(State, 1);
        lua_createtable(State, 0, 2);
        lua_pushvalue(State, -1);
        lua_setglobal(State, "Utils");
    }

    lua_createtable(State, 0, 2);
    
    lua_pushcfunction(State, CLibraries::Patching::PrefixPatch);
    lua_setfield(State, -2, "LoadFile");

    lua_setfield(State, -2, "Patching");
}


int Game::Lua::CLibraries::Patching::PrefixPatch(lua_State* State) {

    luaL_checktype(State, 1, LUA_TFUNCTION);
    luaL_checktype(State, 2, LUA_TFUNCTION);

    if (lua_getmetatable(State, 1) == 0) {

        lua_createtable(State, 0, 3);

        lua_setfield(State, -2, "__call");

        lua_createtable(State, 0, 1);

        lua_pushvalue(State, 2);
        lua_seti(State, -2, 1);

        lua_setfield(State, -2, "_PrefixPatches");

        lua_newtable(State);
        lua_setfield(State, -2, "_SuffixPatches");

        lua_setmetatable(State, 1);
    }
}