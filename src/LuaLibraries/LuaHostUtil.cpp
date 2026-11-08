#include "LuaHostUtil.hpp"

#include <SDL3/SDL_platform_defines.h>

extern "C" {

#ifdef SDL_PLATFORM_VITA
#include <psp2/appmgr.h>
#include <psp2/kernel/processmgr.h>
#include <psp2/power.h>
#include <psp2/vshbridge.h>
#endif

#include "../../include/VM/lualib.h"
}

int Game::Lua::LuaLibraries::HostUtil::GetPlatform(lua_State* State) {
    lua_pushstring(State, "unimplemented");
    return 1;
}

int Game::Lua::LuaLibraries::HostUtil::SetPerformanceMode(lua_State* State) {
    Game::Lua::LuaLibraries::HostUtil::PerformanceMode PerfMode =
        (Game::Lua::LuaLibraries::HostUtil::PerformanceMode)luaL_optinteger(
            State, 1,
            (int)Game::Lua::LuaLibraries::HostUtil::PerformanceMode::Normal);

#ifdef SDL_PLATFORM_VITA

    lua_pushboolean(State, scePowerSetArmClockFrequency((int)PerfMode));

#endif
    return 1;
}