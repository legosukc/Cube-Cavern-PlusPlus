#include "LuaEnums.hpp"

#include "../../include/VM/lua.h"
#include "../../include/VM/lualib.h"

#include <SDL3/SDL_stdinc.h>

#include "../FunctionHeaders/LuaHelper.hpp"

#include <cstdint>
#include <cstring>

static lua_Integer _NextEnumID = INT32_MIN;

static int _EnumIDsReference;

int Game::Lua::CLibraries::Enums::Classes::EnumValue::__index(
    lua_State* State) {
    lua_settop(State, 2);
    if (std::strcmp(luaL_checkstring(State, 2), "ID") == 0) {
        lua_pushinteger(
            State, static_cast<EnumValue*>(
                       LuaHelper::CheckMetatable(State, 1, lua_upvalueindex(1)))
                       ->ID);
    } else {
        lua_gettable(State, lua_upvalueindex(1));
    }

    return 1;
}

int Game::Lua::CLibraries::Enums::FromID(lua_State* State) {
    lua_pushinteger(State, luaL_checkinteger(State, 1));
    lua_gettable(State, ::_EnumIDsReference);
    // lua_geti(State, ::_EnumIDsReference, luaL_checkinteger(State,
    // 1));
    return 1;
}

bool Game::Lua::CLibraries::Enums::RegisterEnumTable(
    lua_State* State,
    int EnumTableStackIndex,
    const char* EnumTableName) {
    lua_getglobal(State, CLibraries::Enums::LibraryName);
    lua_pushstring(State, EnumTableName);
    lua_pushvalue(State, -1);
    if (lua_rawget(State, -3) == LUA_TNIL) {
        lua_pushvalue(State, EnumTableStackIndex);
        lua_rawset(State, -3);

        return true;
    }
    return false;
}

Game::Lua::CLibraries::Enums::Classes::EnumValue*
    Game::Lua::CLibraries::Enums::PushNewEnum(lua_State* State, int Uservalue) {
    Classes::EnumValue* NewEnum = static_cast<Classes::EnumValue*>(
        lua_newuserdata(State, sizeof(Classes::EnumValue)));

    // lua_pushvalue(State, -1);
    // lua_setiuservalue(State, -1, Uservalue);

    lua_pushvalue(State, -1);
    NewEnum->Uservalue = Uservalue;
    NewEnum->ID = lua_ref(State, ::_EnumIDsReference);

    luaL_getmetatable(State, Classes::EnumValue::MetatableName);
    lua_setmetatable(State, -2);

    return NewEnum;
}

void Game::Lua::CLibraries::Enums::Init(lua_State* State) {

    lua_createtable(State, 0, 2);
    luaL_newmetatable(State,
                      CLibraries::Enums::Classes::EnumValue::MetatableName);

    lua_pushvalue(State, -1);
    lua_pushcclosure(State, Classes::EnumValue::__index, "EnumValue.__index", 1);
    lua_setfield(State, -2, "__index");
    
    lua_pop(State, 1);

    lua_createtable(State, 64, 0);
    ::_EnumIDsReference = lua_ref(State, -1);

    lua_createtable(State, 0, 32);

    lua_pushcfunction(State, Enums::FromID, "Enums.FromID");
    lua_setfield(State, -2, "FromID");

    LuaHelper::LockTable(State, -2);

    lua_setglobal(State, CLibraries::Enums::LibraryName);
}