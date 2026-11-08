#include "LuaTypeHelper.hpp"

#include "../../include/VM/lua.h"
#include "../../include/VM/lualib.h"

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