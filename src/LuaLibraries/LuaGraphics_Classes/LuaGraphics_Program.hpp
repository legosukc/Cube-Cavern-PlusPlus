#pragma once

#include "../../define.h"

#include "LuaGraphics_GLObjectBase.hpp"

#include <SDL3/SDL_opengl.h>
#include <SDL3/SDL_opengl_glext.h>

#include <lua.hpp>

namespace Game::Lua::CLibraries::Graphics {

    namespace Classes {
        struct Program
            : Game::Lua::CLibraries::Graphics::Classes::GLObjectBase {
            static int AttachShader(lua_State* State);
            static int DetachShader(lua_State* State);
            static int DetachAllShaders(lua_State* State);

            static int Link(lua_State* State);
            static int Use(lua_State* State);

            static int GetUniformIndex(lua_State* State);

            static int SetUniformBlockBinding(lua_State* State);
            static int GetUniformBlockIndex(lua_State* State);

            static int __gc(lua_State* State);
        };
    }

    namespace Program {
        static inline void Init(lua_State* State,
                                LuaHelper::StackTableReference& GraphicsTable);

        static int __new(lua_State* State);

        static int SetUniformSint32(lua_State* State);
        static int SetUniformUint32(lua_State* State);
        static int SetUniformFloat(lua_State* State);

        static int SetUniformIvec2(lua_State* State);
        static int SetUniformUvec2(lua_State* State);
        static int SetUniformBvec2(lua_State* State);
        static int SetUniformFvec2(lua_State* State);

        static int SetUniformIvec3(lua_State* State);
        static int SetUniformUvec3(lua_State* State);
        static int SetUniformBvec3(lua_State* State);
        static int SetUniformFvec3(lua_State* State);

        static int SetUniformIvec4(lua_State* State);
        static int SetUniformUvec4(lua_State* State);
        static int SetUniformBvec4(lua_State* State);
        static int SetUniformFvec4(lua_State* State);

        static int SetUniformMat2(lua_State* State);
        static int SetUniformMat3(lua_State* State);
        static int SetUniformMat4(lua_State* State);
    }
}

namespace Game::Lua::CLibraries::Graphics::Program {

    int SetUniformSint32(lua_State* State);
    int SetUniformUint32(lua_State* State);
    int SetUniformFloat(lua_State* State);

    int SetUniformIvec2(lua_State* State);
    int SetUniformUvec2(lua_State* State);
    int SetUniformBvec2(lua_State* State);
    int SetUniformFvec2(lua_State* State);

    int SetUniformIvec3(lua_State* State);
    int SetUniformUvec3(lua_State* State);
    int SetUniformBvec3(lua_State* State);
    int SetUniformFvec3(lua_State* State);

    int SetUniformIvec4(lua_State* State);
    int SetUniformUvec4(lua_State* State);
    int SetUniformBvec4(lua_State* State);
    int SetUniformFvec4(lua_State* State);

    int SetUniformMat2(lua_State* State);
    int SetUniformMat3(lua_State* State);
    int SetUniformMat4(lua_State* State);
}