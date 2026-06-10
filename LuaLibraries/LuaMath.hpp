#pragma once

#include <lua-5.5.0/lua.hpp>


namespace Game::Lua::CLibraries::math {
	inline void Init(lua_State* State);

	static int clamp(lua_State* State) {

		int ReturnArgIndex;

		if (lua_compare(State, 1, 2, LUA_OPLT)) {
			ReturnArgIndex = 2;
		} else if (lua_compare(State, 3, 1, LUA_OPLT)) {
			ReturnArgIndex = 3;
		} else {
			ReturnArgIndex = 1;
		}

		lua_settop(State, ReturnArgIndex);
		return 1;
	}
}

void Game::Lua::CLibraries::math::Init(lua_State* State) {

	luaopen_math(State);

	lua_pushcfunction(State, CLibraries::math::clamp);
	lua_setfield(State, -2, "clamp");

	lua_setglobal(State, LUA_MATHLIBNAME);
}