#pragma once

#include <lua.hpp>


namespace Game::Lua::CLibraries::table {
	inline void Init(lua_State* State);

	static int find(lua_State* State);
	static int clear(lua_State* State);
}


void Game::Lua::CLibraries::table::Init(lua_State* State) {

	luaopen_table(State);

	LuaHelper::SetKeyClosure(State, -2, table::find, "find");
	LuaHelper::SetKeyClosure(State, -2, table::clear, "clear");

	lua_setglobal(State, LUA_TABLIBNAME);
}


int Game::Lua::CLibraries::table::find(lua_State* State) {

	lua_settop(State, 2);

	luaL_checktype(State, 1, LUA_TTABLE);
	luaL_argexpected(State, !lua_isnoneornil(State, 2), 2, "any");

	lua_pushnil(State);
	while (lua_next(State, 1) != 0) {

		if (lua_equal(State, -1, 2)) {
			lua_settop(State, 3);
			return 1;
		}
		lua_settop(State, 3);
	}

	return 0;
}

int Game::Lua::CLibraries::table::clear(lua_State* State) {

	luaL_checktype(State, 1, LUA_TTABLE);

	lua_pushnil(State);
	while (lua_next(State, 1) != 0) {

		lua_settop(State, 2);
		lua_pushvalue(State, 2);

		lua_pushnil(State);
		lua_settable(State, 1);
	}

	return 0;
}