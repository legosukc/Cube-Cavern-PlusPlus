#pragma once

#include "../../include/VM/lua.h"

#include "../FunctionHeaders/LuaHelper.hpp"

namespace Game::Lua::CLibraries::TypeHelper {

    inline void Init(lua_State* State);

    /*
     * Gets the type ID from the 1st parameter, the ID returned depends on the
     * LUA_T macros in the source. As such, classes like vectors will either
     * return the 'table' or 'userdata' IDs. This is a Lua function.
     */
    int _typeid(lua_State* State);
    int _typeidname(lua_State* State);
    int _typeof(lua_State* State);

    int iscallable(lua_State* State);
}

void Game::Lua::CLibraries::TypeHelper::Init(lua_State* State) {
    using namespace TypeHelper;

    LuaHelper::SetGlobal(State, _typeid, "typeid");
    LuaHelper::SetGlobal(State, _typeidname, "typeidname");
    LuaHelper::SetGlobal(State, iscallable, "iscallable");
}