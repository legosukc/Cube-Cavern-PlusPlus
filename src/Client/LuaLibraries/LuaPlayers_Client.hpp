#pragma once

#include "../../../include/VM/lua.h"

#include "../Network_Client.hpp"


namespace Game::Lua::CLibraries::Players {
	inline void Init(lua_State* State);
}

void Game::Lua::CLibraries::Players::Init(lua_State* State) {

	lua_createtable(State, 0, 1);

	//Game::Network::
}