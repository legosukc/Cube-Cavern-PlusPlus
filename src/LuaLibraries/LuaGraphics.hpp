#ifndef LUALIBRARIES_LUAGRAPHICS_HPP
#define LUALIBRARIES_LUAGRAPHICS_HPP

#include "../define.h"

#include "../../include/VM/lua.h"

#if !defined(SDL_PLATFORM_VITA) || defined(VITA_USE_PVR)
#include <SDL3/SDL_opengl.h>
#else
#include <vitaGL.h>
#endif

#include "../ConsoleVars.hpp"

namespace Game::ConsoleVars::ReferenceVars::Lua::Graphics {

    struct ChangeErrorCheckingCVar : Game::ConsoleVars::BaseCVar {
        virtual void Set(const char* Argument) override;

        using VarType = void;
    };
}

namespace Game::Lua::CLibraries::Graphics {
    void Init(lua_State* State);
}

#endif