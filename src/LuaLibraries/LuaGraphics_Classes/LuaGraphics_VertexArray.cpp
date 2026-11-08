#include "LuaGraphics_VertexArray.hpp"

#ifdef SDL_PLATFORM_VITA
#include <GLES2/gl2.h>
#include "../../GLES2_typedefs.h"
#else
#include <SDL3/SDL_opengl.h>
#include <SDL3/SDL_opengl_glext.h>
#endif

#include "../../../include/VM/lua.h"
#include "../../../include/VM/lualib.h"

#include "../../FunctionHeaders/LuaHelper.hpp"

#include "../../Client/GraphicsClasses/VertexArray.hpp"

int Game::Lua::CLibraries::Graphics::VertexArray::__new(lua_State* State) {
    Classes::VertexArray* VertexArrayUD = static_cast<Classes::VertexArray*>(
        lua_newuserdata(State, sizeof(Classes::VertexArray)));
    Game::Graphics::OpenGLFunctions::glGenVertexArrays(
        1, &VertexArrayUD->GLObject);
    luaL_getmetatable(State, "VertexArray");
    lua_setmetatable(State, -2);
    return 1;
}

int Game::Lua::CLibraries::Graphics::Classes::VertexArray::Bind(
    lua_State* State) {
    Game::Graphics::OpenGLFunctions::glBindVertexArray(
        static_cast<Classes::VertexArray*>(
            LuaHelper::CheckMetatable(State, 1, lua_upvalueindex(1)))
            ->GLObject);
    return 0;
}

int Game::Lua::CLibraries::Graphics::VertexArray::EnableAttribute(
    lua_State* State) {
    Game::Graphics::OpenGLFunctions::glEnableVertexAttribArray(
        static_cast<GLuint>(luaL_checkinteger(State, 1)));
    return 0;
}

int Game::Lua::CLibraries::Graphics::VertexArray::DisableAttribute(
    lua_State* State) {
    Game::Graphics::OpenGLFunctions::glDisableVertexAttribArray(
        static_cast<GLuint>(luaL_checkinteger(State, 1)));
    return 0;
}

int Game::Lua::CLibraries::Graphics::VertexArray::SetAttributePointer(
    lua_State* State) {
    Game::Graphics::OpenGLFunctions::glVertexAttribPointer(
        static_cast<GLuint>(luaL_checkinteger(State, 1)),     // Index
        static_cast<GLint>(luaL_checkinteger(State, 2)),      // Size
        static_cast<GLenum>(luaL_checkinteger(State, 3)),     // Type
        static_cast<GLboolean>(lua_toboolean(State, 4)),      // Normalized
        static_cast<GLsizei>(luaL_checkinteger(State, 5)),    // Stride Size
        reinterpret_cast<void*>(luaL_checkinteger(State, 6))  // Offset
    );
    return 0;
}

int Game::Lua::CLibraries::Graphics::VertexArray::Unbind(lua_State* State) {
    Game::Graphics::OpenGLFunctions::glBindVertexArray(0);
    return 0;
}

int Game::Lua::CLibraries::Graphics::Classes::VertexArray::__gc(
    lua_State* State) {
    Game::Graphics::OpenGLFunctions::glDeleteVertexArrays(
        1, &static_cast<Classes::VertexArray*>(
                luaL_checkudata(State, 1, "VertexArray"))
                ->GLObject);
    return 0;
}
