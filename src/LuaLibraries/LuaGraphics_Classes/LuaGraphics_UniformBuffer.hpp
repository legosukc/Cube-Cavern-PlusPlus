#pragma once

#include "LuaGraphics_BufferBase.hpp"

namespace Game::Lua::CLibraries::Graphics::Classes {
    struct UniformBuffer
        : Game::Lua::CLibraries::Graphics::Classes::BufferBase {
        static inline void InitMetatable(lua_State* State);

        static int BindToUniformBlockIndex(lua_State* State);
        static int BindToUniformBlockIndexRange(lua_State* State);
    };
}

void Game::Lua::CLibraries::Graphics::Classes::UniformBuffer::InitMetatable(
    lua_State* State) {
    LuaHelper::StackTableReference UniformBufferMetatable =
        BufferBase::InitMetatable(State, "UniformBuffer");

    UniformBufferMetatable.PushReference(State);
    UniformBufferMetatable.SetKeyClosure(State,
                                         UniformBuffer::BindToUniformBlockIndex,
                                         "BindToUniformBlockIndex", 1);

    UniformBufferMetatable.PushReference(State);
    UniformBufferMetatable.SetKeyClosure(
        State, UniformBuffer::BindToUniformBlockIndexRange,
        "BindToUniformBlockIndexRange", 1);

    lua_settop(State, UniformBufferMetatable.GetStackIndex() - 1);
}

namespace Game::Lua::CLibraries::Graphics::UniformBuffer {
    using namespace Game::Lua::CLibraries::Graphics::BufferBase;
}