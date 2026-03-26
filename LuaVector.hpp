#pragma once

#include <lua-5.4.2/lua.hpp>

namespace Game::Lua::CLibraries::Vector {

	namespace Vector3 {

		int __new(lua_State* State);

		int __index(lua_State* State);
		int __newindex(lua_State* State);

		int __add(lua_State* State);
		int __sub(lua_State* State);
		int __mul(lua_State* State);
		int __div(lua_State* State);
		int __mod(lua_State* State);
	}

	void Init(lua_State* State);
}