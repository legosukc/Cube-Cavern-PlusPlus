#include "LuaGraphics_UniformBuffer.hpp"

int Game::Lua::CLibraries::Graphics::Classes::UniformBuffer::
    BindToUniformBlockIndex(lua_State* State) {
#ifdef SDL_PLATFORM_VITA
    luaL_error(State,
               "Function 'BindToUniformBlockIndex' is not implemented "
               "on this platform.");
#else
    Game::Graphics::OpenGLFunctions::glBindBufferBase(
        GL_UNIFORM_BUFFER,
        static_cast<GLuint>(luaL_checkinteger(State, 2)),  // UniformBlockIndex
        static_cast<UniformBuffer*>(luaL_checkudata(State, 1, "UniformBuffer"))
            ->GLObject);
#endif
    return 0;
}

int Game::Lua::CLibraries::Graphics::Classes::UniformBuffer::
    BindToUniformBlockIndexRange(lua_State* State) {
#ifdef SDL_PLATFORM_VITA
    luaL_error(State,
               "Function 'BindToUniformBlockIndexRange' is not implemented "
               "on this platform.");
#else
    Game::Graphics::OpenGLFunctions::glBindBufferRange(
        GL_UNIFORM_BUFFER,
        static_cast<GLuint>(luaL_checkinteger(State, 2)),  // UniformBlockIndex
        static_cast<UniformBuffer*>(luaL_checkudata(State, 1, "UniformBuffer"))
            ->GLObject,
        static_cast<GLintptr>(luaL_checkinteger(State, 3)),   // Offset
        static_cast<GLsizeiptr>(luaL_checkinteger(State, 4))  // Size
    );
#endif
    return 0;
}

namespace Game::Lua::CLibraries::Graphics::UniformBuffer {
    using namespace Game::Lua::CLibraries::Graphics::BufferBase;
}