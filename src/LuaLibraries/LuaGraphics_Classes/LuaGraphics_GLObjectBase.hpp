#pragma once

#include "../../define.h"

#include <SDL3/SDL_platform_defines.h>

#ifdef SDL_PLATFORM_VITA
#include <GLES2/gl2.h>
#else
#include <SDL3/SDL_opengl.h>
#endif

#include "../../../include/VM/lua.h"

#include "../../FunctionHeaders/LuaHelper.hpp"

namespace Game::Lua::CLibraries::Graphics::Classes {

    struct GLObjectBase {
        static LuaHelper::StackTableReference InitMetatable(
            lua_State* State,
            const char* MetatableName);

        GLuint GLObject;

        static int __gc(lua_State* State);
    };
};