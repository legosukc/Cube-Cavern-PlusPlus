#pragma once

#include "../../include/VM/lua.h"

namespace Game::Lua::CLibraries::Network {
    inline void Init(lua_State* State);
    inline void Destroy(lua_State* State);

    int GetRemotePool(lua_State* State);

    struct RemoteEvent {
        static inline void InitMetatable(lua_State* State);
    };
}

void Game::Lua::CLibraries::Network::Init(lua_State* State) {
    lua_getglobal(State, "Game");
    lua_createtable(State, 0, 1);
    lua_setfield(State, -2, "Network");
}

void Game::Lua::CLibraries::Network::RemoteEvent::InitMetatable(
    lua_State* State) {}

void Game::Lua::CLibraries::Network::Destroy(lua_State* State) {}