#pragma once

#include "../../define.h"

#include "LuaGraphics_GLObjectBase.hpp"

#ifdef SDL_PLATFORM_VITA
#include <GLES2/gl2.h>
#else
#include <SDL3/SDL_opengl.h>
#include <SDL3/SDL_opengl_glext.h>
#endif

#include <SDL3/SDL_stdinc.h>

#include "../../../include/VM/lua.h"
#include "../../../include/VM/lualib.h"

#include "../../FunctionHeaders/LuaHelper.hpp"

#include "../../Client/Graphics_Client.hpp"

namespace Game::Lua::CLibraries::Graphics {

    namespace Classes {
        struct BufferBase
            : Game::Lua::CLibraries::Graphics::Classes::GLObjectBase {
            GLenum BufferType;

            static int Bind(lua_State* State);

            static int __gc(lua_State* State);
        };
    }

    namespace BufferBase {
        template <GLenum BufferType>
        inline void Init(lua_State* State,
                         const char* Name,
                         LuaHelper::StackTableReference& GraphicsTable);

        template <GLenum BufferType>
        int __new(lua_State* State);

        template <GLenum BufferType>
        int CopyFromBuffer(lua_State* State);

        template <GLenum BufferType>
        int AllocateBuffer(lua_State* State);

        template <GLenum BufferType>
        int CopyFromBufferToPointer(lua_State* State);

        template <GLenum BufferType>
        int CopyToPointer(lua_State* State);

        template <GLenum BufferType>
        int Unbind(lua_State* State);
    }
}

template <GLenum BufferType>
void Game::Lua::CLibraries::Graphics::BufferBase::Init(
    lua_State* State,
    const char* Name,
    LuaHelper::StackTableReference& GraphicsTable) {
    LuaHelper::StackTableReference BufferBaseTable, Metatable;

    BufferBaseTable = LuaHelper::StackTableReference(State, 0, 2);
    Metatable = LuaHelper::StackTableReference(State, Name);

    Metatable.SetKeyClosure(State, Classes::BufferBase::__gc, "__gc");

    Metatable.PushReference(State);
    Metatable.SetKeyClosure(State, Classes::BufferBase::Bind, "Bind", 1);

    Metatable.PushReference(State);
    lua_setfield(State, Metatable.GetStackIndex(), "__index");

    Metatable.PushReference(State);
    BufferBaseTable.SetKeyClosure(State, BufferBase::__new<BufferType>, "new",
                                  1);

    lua_settop(State, BufferBaseTable.GetStackIndex());

    BufferBaseTable.SetKeyClosure(State, BufferBase::CopyFromBuffer<BufferType>,
                                  "CopyFromBuffer");
    BufferBaseTable.SetKeyClosure(
        State, BufferBase::CopyFromBufferToPointer<BufferType>,
        "CopyFromBufferToPointer");

    BufferBaseTable.SetKeyClosure(State, BufferBase::CopyToPointer<BufferType>,
                                  "CopyToPointer");

    BufferBaseTable.SetKeyClosure(State, BufferBase::AllocateBuffer<BufferType>,
                                  "AllocateBuffer");

    BufferBaseTable.SetKeyClosure(State, BufferBase::Unbind<BufferType>,
                                  "Unbind");

    lua_setfield(State, GraphicsTable.GetStackIndex(), Name);
}