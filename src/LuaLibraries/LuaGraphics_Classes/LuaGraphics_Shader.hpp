#pragma once

#include <stdlib.h>

#include <SDL3/SDL_platform_defines.h>
#include <SDL3/SDL_timer.h>

#ifdef SDL_PLATFORM_VITA
#include <GLES2/gl2.h>
#else
#include <SDL3/SDL_opengl.h>
#include <SDL3/SDL_opengl_glext.h>
#endif

#include "../../../include/VM/lua.h"

#include "../../FunctionHeaders/LuaHelper.hpp"

namespace Game::Lua::CLibraries::Graphics {

    namespace Classes {
        struct Shader {
            GLint ShaderObject;
            GLenum ShaderType;

            static int SetShaderSource(lua_State* State);
            static int LoadShaderSourceFromFile(lua_State* State);

            static int GetShaderSource(lua_State* State);

            static int Compile(lua_State* State);

            static int __gc(lua_State* State);
        };
    }

    namespace Shader {

        constexpr char MetatableName[] = "Shader";

        inline void Init(lua_State* State,
                                LuaHelper::StackTableReference& GraphicsTable);

        // not safe to directly c call
        int __new(lua_State* State);
    }
}

void Game::Lua::CLibraries::Graphics::Shader::Init(
    lua_State* State,
    LuaHelper::StackTableReference& GraphicsTable) {
    LuaHelper::StackTableReference OpenGLShader, ShaderMetatable;

    ShaderMetatable = LuaHelper::StackTableReference(
        State, CLibraries::Graphics::Shader::MetatableName);

    ShaderMetatable.SetKeyClosure(State, Classes::Shader::SetShaderSource,
                                  "SetShaderSource");
    ShaderMetatable.SetKeyClosure(State,
                                  Classes::Shader::LoadShaderSourceFromFile,
                                  "LoadShaderSourceFromFile");

    ShaderMetatable.SetKeyClosure(State, Classes::Shader::GetShaderSource,
                                  "GetShaderSource");

    ShaderMetatable.SetKeyClosure(State, Classes::Shader::Compile, "Compile");

    ShaderMetatable.SetKeyClosure(State, Classes::Shader::__gc, "__gc");
    // ShaderMetatable.SetKeyClosure(State, Classes::Shader::__eq, "__eq");

    ShaderMetatable.PushReference(State);
    lua_setfield(State, ShaderMetatable.GetStackIndex(), "__index");

    lua_settop(State, ShaderMetatable.GetStackIndex() - 1);

    OpenGLShader = LuaHelper::StackTableReference(State, 0, 1);

    OpenGLShader.SetKeyClosure(State, Shader::__new, "new");

    lua_setfield(State, GraphicsTable.GetStackIndex(), "Shader");
}