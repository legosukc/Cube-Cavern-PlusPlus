#pragma once

#include <SDL3/SDL_platform_defines.h>

#include "../../../include/VM/lua.h"

#ifdef SDL_PLATFORM_3DS
#include "RenderObjectImplementations/3ds.h"

#elif SDL_PLATFORM_VITA
#include "RenderObjectImplementations/gxm.h"

#endif

namespace Game::Lua::CLibraries::RenderObjectImplementations {
    inline void Init(lua_State* State);
}

void Game::Lua::CLibraries::RenderObjectImplementations::Init(
    lua_State* State) {
#ifdef SDL_PLATFORM_3DS

#elif SDL_PLATFORM_VITA

#endif
}