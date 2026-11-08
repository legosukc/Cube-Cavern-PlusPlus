#include "LuaNetwork.hpp"

#include "../../include/VM/lua.h"


int Game::Lua::CLibraries::Network::GetRemotePool(lua_State* State) {
    return 1;
}