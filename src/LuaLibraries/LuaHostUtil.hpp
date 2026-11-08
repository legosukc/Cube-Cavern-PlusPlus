#pragma once

#include <SDL3/SDL_platform_defines.h>

extern "C" {
#include "../../include/VM/lua.h"
}

namespace Game::Lua::LuaLibraries::HostUtil {
    inline void Init(lua_State* State);

    enum class PerformanceMode {
#ifdef SDL_PLATFORM_VITA
        Lowest = 166,
        Low = 222,
        Normal = 333,
        High = 444,
        Highest = 444
#else
        Lowest,
        Low,
        Normal,
        High,
        Highest
#endif
    };

    int GetPlatform(lua_State* State);
    int SetPerformanceMode(lua_State* State);
}

void Game::Lua::LuaLibraries::HostUtil::Init(lua_State* State) {}