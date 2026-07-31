#pragma once

#include "../../include/VM/lua.h"
#include "../../include/VM/lualib.h"

#include "../FunctionHeaders/LuaHelper.hpp"

namespace Game::Lua::CLibraries::TypeHelper {

    inline void Init(lua_State* State);

    /*
     * Gets the type ID from the 1st parameter, the ID returned depends on the
     * LUA_T macros in the source. As such, classes like vectors will either
     * return the 'table' or 'userdata' IDs. This is a Lua function.
     */
    static int _typeid(lua_State* State);
    static int _typeidname(lua_State* State);
    static int _typeof(lua_State* State);

    static int iscallable(lua_State* State);
}

void Game::Lua::CLibraries::TypeHelper::Init(lua_State* State) {
    using namespace TypeHelper;

    LuaHelper::SetGlobal(State, _typeid, "typeid");
    LuaHelper::SetGlobal(State, _typeidname, "typeidname");
    LuaHelper::SetGlobal(State, iscallable, "iscallable");
}

int Game::Lua::CLibraries::TypeHelper::_typeid(lua_State* State) {
    lua_pushinteger(State, static_cast<lua_Integer>(lua_type(State, 1)));
    return 1;
}

int Game::Lua::CLibraries::TypeHelper::_typeidname(lua_State* State) {
    lua_pushstring(State, lua_typename(State, luaL_checkinteger(State, 1)));
    return 1;
}

int Game::Lua::CLibraries::TypeHelper::iscallable(lua_State* State) {
    int Result;
    switch (lua_type(State, 1)) {
        case LUA_TFUNCTION:
            Result = true;
            break;

        case LUA_TUSERDATA:
        case LUA_TTABLE:
            Result = luaL_getmetafield(State, 1, "__call") == LUA_TFUNCTION;
            break;
        default:
            Result = false;
    }

    lua_pushboolean(State, Result);
    return 1;
}