#pragma once

#include "../../include/VM/lua.h"

#include <SDL3/SDL_stdinc.h>


namespace Game::Lua::CLibraries::Patching {
	inline void Init(lua_State* State);

	int PrefixPatch(lua_State* State);
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
    
    lua_pushcfunction(State, CLibraries::Patching::PrefixPatch, "PrefixPatch");
    lua_setfield(State, -2, "LoadFile");

    lua_setfield(State, -2, "Patching");
}