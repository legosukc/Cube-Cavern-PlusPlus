#pragma once

#include "LuaGraphics_GLObjectBase.hpp"

#include <SDL3/SDL_joystick.h>

#ifdef SDL_PLATFORM_VITA
#include <GLES2/gl2.h>
#include "../../GLES2_typedefs.h"
#else
#include <SDL3/SDL_opengl.h>
#include <SDL3/SDL_opengl_glext.h>
#endif

#include "../../../include/VM/lua.h"

namespace Game::Lua::CLibraries::Graphics {

    namespace Classes {
        struct Program
            : Game::Lua::CLibraries::Graphics::Classes::GLObjectBase {
            static int AttachShader(lua_State* State);
            static int DetachShader(lua_State* State);
            static int DetachAllShaders(lua_State* State);

            static int LoadVertexShaderFile(lua_State* State);
            static int LoadFragmentShaderFile(lua_State* State);

            static int Link(lua_State* State);
            static int Use(lua_State* State);

            static int GetUniformIndex(lua_State* State);

            static int SetUniformBlockBinding(lua_State* State);
            static int GetUniformBlockIndex(lua_State* State);

            static int __gc(lua_State* State);
        };
    }

    namespace Program {
        inline void Init(lua_State* State,
                         LuaHelper::StackTableReference& GraphicsTable);

        // safe to directly c call
        int __new(lua_State* State);

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
}

void Game::Lua::CLibraries::Graphics::Program::Init(
    lua_State* State,
    LuaHelper::StackTableReference& GraphicsTable) {
    LuaHelper::StackTableReference ProgramMetatable(State, "Program");

    ProgramMetatable.SetKeyClosure(State, Classes::Program::__gc, "__gc");

    ProgramMetatable.PushReference(State);
    lua_setfield(State, ProgramMetatable.GetStackIndex(), "__index");

    ProgramMetatable.SetKeyClosure(State, Classes::Program::GetUniformIndex,
                                   "GetUniformIndex");

    ProgramMetatable.SetKeyClosure(State,
                                   Classes::Program::SetUniformBlockBinding,
                                   "SetUniformBlockBinding");
    ProgramMetatable.SetKeyClosure(
        State, Classes::Program::GetUniformBlockIndex, "GetUniformBlockIndex");

    ProgramMetatable.SetKeyClosure(State, Classes::Program::AttachShader,
                                   "AttachShader");
    ProgramMetatable.SetKeyClosure(State, Classes::Program::DetachShader,
                                   "DetachShader");
    ProgramMetatable.SetKeyClosure(State, Classes::Program::DetachAllShaders,
                                   "DetachAllShaders");

    ProgramMetatable.SetKeyClosure(State, Classes::Program::Link, "Link");

    ProgramMetatable.PushReference(State);
    ProgramMetatable.SetKeyClosure(State, Classes::Program::Use, "Use", 1);

    lua_settop(State, ProgramMetatable.GetStackIndex() - 1);

    LuaHelper::StackTableReference ProgramTable(State, 0,
                                                17);  // OpenGL.Program

    ProgramTable.SetKeyClosure(State, CLibraries::Graphics::Program::__new,
                               "new");

    ProgramTable.SetKeyClosure(State,
                               CLibraries::Graphics::Program::SetUniformSint32,
                               "SetUniformSint32");
    ProgramTable.SetKeyClosure(State,
                               CLibraries::Graphics::Program::SetUniformUint32,
                               "SetUniformUint32");

    // TODO: change this to use a SetUniformBool thing
    ProgramTable.SetKeyClosure(State,
                               CLibraries::Graphics::Program::SetUniformSint32,
                               "SetUniformBool");
    ProgramTable.SetKeyClosure(State,
                               CLibraries::Graphics::Program::SetUniformFloat,
                               "SetUniformFloat");

    ProgramTable.SetKeyClosure(State,
                               CLibraries::Graphics::Program::SetUniformIvec2,
                               "SetUniformIvec2");
    ProgramTable.SetKeyClosure(State,
                               CLibraries::Graphics::Program::SetUniformUvec2,
                               "SetUniformUvec2");
    ProgramTable.SetKeyClosure(State,
                               CLibraries::Graphics::Program::SetUniformBvec2,
                               "SetUniformBvec2");
    ProgramTable.SetKeyClosure(State,
                               CLibraries::Graphics::Program::SetUniformFvec2,
                               "SetUniformVec2");

    ProgramTable.SetKeyClosure(State,
                               CLibraries::Graphics::Program::SetUniformIvec3,
                               "SetUniformIvec3");
    ProgramTable.SetKeyClosure(State,
                               CLibraries::Graphics::Program::SetUniformUvec3,
                               "SetUniformUvec3");
    ProgramTable.SetKeyClosure(State,
                               CLibraries::Graphics::Program::SetUniformBvec3,
                               "SetUniformBvec3");
    ProgramTable.SetKeyClosure(State,
                               CLibraries::Graphics::Program::SetUniformFvec3,
                               "SetUniformVec3");

    ProgramTable.SetKeyClosure(State,
                               CLibraries::Graphics::Program::SetUniformIvec4,
                               "SetUniformIvec4");
    ProgramTable.SetKeyClosure(State,
                               CLibraries::Graphics::Program::SetUniformUvec4,
                               "SetUniformUvec4");
    ProgramTable.SetKeyClosure(State,
                               CLibraries::Graphics::Program::SetUniformBvec4,
                               "SetUniformBvec4");
    ProgramTable.SetKeyClosure(State,
                               CLibraries::Graphics::Program::SetUniformFvec4,
                               "SetUniformVec4");

    ProgramTable.SetKeyClosure(
        State, CLibraries::Graphics::Program::SetUniformMat2, "SetUniformMat2");
    ProgramTable.SetKeyClosure(
        State, CLibraries::Graphics::Program::SetUniformMat3, "SetUniformMat3");
    ProgramTable.SetKeyClosure(
        State, CLibraries::Graphics::Program::SetUniformMat4, "SetUniformMat4");

    lua_setfield(State, GraphicsTable.GetStackIndex(), "Program");
}