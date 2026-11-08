#pragma once

#include "../../include/VM/lua.h"

#include "../FunctionHeaders/LuaHelper.hpp"

#include <SDL3/SDL_stdinc.h>

namespace Game::Lua::CLibraries::Enums {

    namespace etc {
        extern int _EnumIDsReference;
    }

    inline void Init(lua_State* State);

    constexpr const char* const LibraryName = "Enums";

    namespace Classes {
        struct EnumValue {
            static constexpr const char* MetatableName = "Enum";
			
			int Uservalue;
            int ID;

            static int __index(lua_State* State);
        };
    }
    
    int FromID(lua_State* State);

    bool RegisterEnumTable(lua_State* State,
                           int EnumTableStackIndex,
                           const char* EnumTableName);

    Classes::EnumValue* PushNewEnum(lua_State* State, int Uservalue = 0);
}

void Game::Lua::CLibraries::Enums::Init(lua_State* State) {
    lua_createtable(State, 0, 2);
    luaL_newmetatable(State,
                      CLibraries::Enums::Classes::EnumValue::MetatableName);

    lua_pushvalue(State, -1);
    lua_pushcclosure(State, Classes::EnumValue::__index, "EnumValue.__index",
                     1);
    lua_setfield(State, -2, "__index");

    lua_pop(State, 1);

    lua_createtable(State, 64, 0);
    etc::_EnumIDsReference = lua_ref(State, -1);

    lua_createtable(State, 0, 32);

    lua_pushcfunction(State, Enums::FromID, "Enums.FromID");
    lua_setfield(State, -2, "FromID");

    LuaHelper::LockTable(State, -2);

    lua_setglobal(State, CLibraries::Enums::LibraryName);
}