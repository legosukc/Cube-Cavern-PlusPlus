#pragma once

#include "LuaGraphics_GLObjectBase.hpp"

#include <SDL3/SDL_opengl_glext.h>
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

template <GLenum BufferType>
int Game::Lua::CLibraries::Graphics::BufferBase::__new(lua_State* State) {
    Classes::BufferBase* BufferUD = static_cast<Classes::BufferBase*>(
        lua_newuserdata(State, sizeof(Classes::BufferBase)));
    BufferUD->BufferType = BufferType;

    Game::Graphics::OpenGLFunctions::glGenBuffers(1, &BufferUD->GLObject);

    lua_pushvalue(State, lua_upvalueindex(1));
    lua_setmetatable(State, -2);

    return 1;
}

template <GLenum BufferType>
int Game::Lua::CLibraries::Graphics::BufferBase::Unbind(lua_State* State) {
    Game::Graphics::OpenGLFunctions::glBindBuffer(BufferType, 0);
    return 0;
}

template <GLenum BufferType>
int Game::Lua::CLibraries::Graphics::BufferBase::CopyFromBuffer(
    lua_State* State) {
    // using BufferStruct = Game::Lua::CLibraries::Buffer::Classes::Buffer;

    char* Buffer;
    size_t UploadSize;

    Buffer = static_cast<char*>(luaL_checkbuffer(State, 1, &UploadSize));

    if (!lua_isnoneornil(State, 2)) {
        const size_t SpecifiedUploadSize =
            static_cast<size_t>(luaL_checknumber(State, 2));

        unlikely_branch if (SpecifiedUploadSize > UploadSize) {
            luaL_error(State,
                       "Attempted to allocate a OpenGL buffer in 'BufferData' "
                       "with a UploadSize bigger than the provided buffer.");
        }
    }

    Game::Graphics::OpenGLFunctions::glBufferData(
        BufferType, UploadSize, Buffer,
        luaL_optinteger(State, 3, GL_STATIC_DRAW));
    return 0;
}

template <GLenum BufferType>
int Game::Lua::CLibraries::Graphics::BufferBase::AllocateBuffer(
    lua_State* State) {
    Game::Graphics::OpenGLFunctions::glBufferData(
        BufferType, luaL_checkinteger(State, 1), NULL,
        static_cast<GLenum>(luaL_optnumber(State, 2, GL_STATIC_DRAW)));
    return 0;
}

template <GLenum BufferType>
int Game::Lua::CLibraries::Graphics::BufferBase::CopyFromBufferToPointer(
    lua_State* State) {
    //using BufferStruct = Game::Lua::CLibraries::Buffer::Classes::Buffer;

    char* Buffer;
    size_t UploadSize;

    Buffer = static_cast<char*>(luaL_checkbuffer(State, 1, &UploadSize));

    if (!lua_isnoneornil(State, 3)) {
        const size_t SpecifiedUploadSize = static_cast<size_t>(luaL_checkinteger64(State, 3));

        unlikely_branch if (SpecifiedUploadSize > UploadSize) {
            luaL_error(State,
                       "Attempted to allocate a OpenGL buffer in 'BufferData' "
                       "with a UploadSize bigger than the provided buffer.");
        }
    }

    Game::Graphics::OpenGLFunctions::glBufferSubData(
        BufferType, luaL_checkinteger(State, 2), UploadSize, Buffer);
    return 0;
}

template <GLenum BufferType>
int Game::Lua::CLibraries::Graphics::BufferBase::CopyToPointer(
    lua_State* State) {
    const void* Data = lua_touserdata(State, 1);

    Game::Graphics::OpenGLFunctions::glBufferSubData(
        BufferType, luaL_checkinteger(State, 2),
        *(static_cast<const size_t*>(Data) - sizeof(size_t)), Data);
    return 0;
}

int Game::Lua::CLibraries::Graphics::Classes::BufferBase::Bind(
    lua_State* State) {
    const BufferBase* const BufferUD = static_cast<BufferBase*>(
        LuaHelper::CheckMetatable(State, 1, lua_upvalueindex(1)));
    Game::Graphics::OpenGLFunctions::glBindBuffer(BufferUD->BufferType,
                                                  BufferUD->GLObject);
    return 0;
}
int Game::Lua::CLibraries::Graphics::Classes::BufferBase::__gc(
    lua_State* State) {
    Game::Graphics::OpenGLFunctions::glDeleteBuffers(
        1, &static_cast<BufferBase*>(
                LuaHelper::CheckMetatable(State, 1, lua_upvalueindex(1)))
                ->GLObject);
    return 0;
}