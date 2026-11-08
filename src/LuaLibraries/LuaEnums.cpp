#include "LuaEnums.hpp"

#include "../../include/VM/lua.h"
#include "../../include/VM/lualib.h"

#include <SDL3/SDL_stdinc.h>

#include "../FunctionHeaders/LuaHelper.hpp"

#include <cstdint>
#include <cstring>

static lua_Integer _NextEnumID = INT32_MIN;

namespace Game::Lua::CLibraries::Enums::etc {
    int _EnumIDsReference;
}

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
    lua_gettable(State, etc::_EnumIDsReference);
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
    NewEnum->ID = lua_ref(State, etc::_EnumIDsReference);

    luaL_getmetatable(State, Classes::EnumValue::MetatableName);
    lua_setmetatable(State, -2);

    return NewEnum;
}