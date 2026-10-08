#ifndef LUALIBRARIES_LUACONSOLE_HPP
#define LUALIBRARIES_LUACONSOLE_HPP

#include "../../include/VM/lua.h"

#include <SDL3/SDL_stdinc.h>

namespace Game::Lua::CLibraries::Console {

	inline void Init(lua_State* State);
	inline void PostMiscClassInit(lua_State* State);

	inline void DestroyLuaLibrary();
	inline void DestroyLibrary();

	inline void Update(lua_State* State);

	inline constexpr const char* LibraryName = "Console";
}

#include "LuaConsole.cpp"

#endif