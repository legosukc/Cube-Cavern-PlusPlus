#pragma once

#include "../../../include/VM/lua.h"
#include "../../FunctionHeaders/LuaHelper.hpp"

namespace Game::Lua::CLibraries::Network {
    inline void Init(lua_State* State);
    inline void Destroy(lua_State* State);

    enum class LuaTypeMappings {
        Nil,
        Integer,
        Number,
        Boolean,
        String,
        Function,
        Table,
        Buffer,
        Vector2,
        IVector2,
        UVector2,
        Vector3,
        IVector3,
        UVector3,
        Vector4,
        IVector4,
        UVector4,
        Enum,
    };

    int GetRemotePool(lua_State* State);

    struct RemoteEvent {
        static inline void InitMetatable(lua_State* State);

        // TODO: implement fully
        static int Fire(lua_State* State);
    };
}

void Game::Lua::CLibraries::Network::Init(lua_State* State) {
    LuaHelper::StackTableReference PacketsTable(State, 0, 1);

    lua_getglobal(State, "Game");
    lua_pushvalue(State, PacketsTable.GetStackIndex());
    lua_setfield(State, PacketsTable.GetStackIndex() + 1, "Network");
}