#include "LuaGraphics.hpp"

#include "../define.h"

#include "../../include/VM/lua.h"

#ifdef SDL_PLATFORM_VITA
#include <GLES2/gl2.h>
#include "../GLES2_typedefs.h"
#else
#include <SDL3/SDL_opengl.h>
#endif

#ifdef BUILD_CLIENT

#include "../FunctionHeaders/MathTransform.hpp"
#endif

// #include "../Lua.hpp"

namespace Game::Lua {
    extern lua_State* State;
}

void Game::ConsoleVars::ReferenceVars::Lua::Graphics::ChangeErrorCheckingCVar::
    SetWithString(const char* NewValue) {}

#ifdef BUILD_CLIENT
int Game::Lua::CLibraries::Graphics::etc::DrawArrays(lua_State* State) {
    Game::Graphics::OpenGLFunctions::glDrawArrays(
        static_cast<GLenum>(luaL_checkinteger(State, 1)),  // Mode
        static_cast<GLint>(luaL_optinteger(State, 2, 0)),  // First
        static_cast<GLsizei>(luaL_checkinteger(State, 3))  // Count
    );
    return 0;
}

int Game::Lua::CLibraries::Graphics::etc::DrawElements(lua_State* State) {
    Game::Graphics::OpenGLFunctions::glDrawElements(
        static_cast<GLenum>(luaL_checkinteger(State, 1)),   // Mode
        static_cast<GLsizei>(luaL_checkinteger(State, 2)),  // Count
        static_cast<GLenum>(luaL_checkinteger(State, 3)),   // Type
        reinterpret_cast<const void*>(                      // Offset
            luaL_optinteger(State, 4, 0)));
    return 0;
}

int Game::Lua::CLibraries::Graphics::etc::DrawArraysInstanced(
    lua_State* State) {
    Game::Graphics::OpenGLFunctions::glDrawArraysInstanced(
        static_cast<GLenum>(luaL_checkinteger(State, 1)),   // Mode
        static_cast<GLint>(luaL_optinteger(State, 2, 0)),   // First
        static_cast<GLsizei>(luaL_checkinteger(State, 3)),  // Count
        static_cast<GLsizei>(luaL_checkinteger(State, 4))   // Instance Count
    );
    return 0;
}

int Game::Lua::CLibraries::Graphics::etc::DrawElementsInstanced(
    lua_State* State) {
    Game::Graphics::OpenGLFunctions::glDrawElementsInstanced(
        static_cast<GLenum>(luaL_checkinteger(State, 1)),   // Mode
        static_cast<GLsizei>(luaL_checkinteger(State, 2)),  // Count
        static_cast<GLenum>(luaL_checkinteger(State, 3)),   // Type
        reinterpret_cast<const void*>(                      // Offset
            luaL_optinteger(State, 4, 0)),
        static_cast<GLsizei>(luaL_checkinteger(State, 5))  // Instance Count
    );
    return 0;
}

int Game::Lua::CLibraries::Graphics::etc::SetClearColor(lua_State* State) {
    Game::Graphics::SetClearColor(
        static_cast<float>(luaL_optnumber(State, 1, 0.0)),
        static_cast<float>(luaL_optnumber(State, 2, 0.0)),
        static_cast<float>(luaL_optnumber(State, 3, 0.0)),
        static_cast<float>(luaL_optnumber(State, 4, 0.0)));
    return 0;
}

int Game::Lua::CLibraries::Graphics::etc::Clear(lua_State* State) {
    Game::Graphics::ClearBitfields(Game::Graphics::BufferBitfields.ColorBit |
                                   Game::Graphics::BufferBitfields.DepthBit |
                                   Game::Graphics::BufferBitfields.StencilBit);
    return 0;
}

template <GLenum Feature>
int Game::Lua::CLibraries::Graphics::etc::Enable_feature(lua_State* State) {
    Game::Graphics::OpenGLFunctions::glEnable(Feature);
    return 0;
}

template int Game::Lua::CLibraries::Graphics::etc::Enable_feature<
    GL_DEPTH_TEST>(lua_State* State);

template int Game::Lua::CLibraries::Graphics::etc::Enable_feature<
    GL_STENCIL_TEST>(lua_State* State);

template int Game::Lua::CLibraries::Graphics::etc::Enable_feature<
    GL_BLEND>(lua_State* State);

template <GLenum Feature>
int Game::Lua::CLibraries::Graphics::etc::Disable_feature(lua_State* State) {
    Game::Graphics::OpenGLFunctions::glDisable(Feature);
    return 0;
}

template int Game::Lua::CLibraries::Graphics::etc::Disable_feature<
    GL_DEPTH_TEST>(lua_State* State);

template int Game::Lua::CLibraries::Graphics::etc::Disable_feature<
    GL_STENCIL_TEST>(lua_State* State);

template int Game::Lua::CLibraries::Graphics::etc::Disable_feature<GL_BLEND>(
    lua_State* State);

void Game::Lua::CLibraries::Graphics::etc::_lua_graphics_updateScreenSize(
    int TableIndex,
    const Math::IVector2& Size) {
    lua_pushinteger(Game::Lua::State, Size.X);
    lua_setfield(Game::Lua::State, TableIndex, "ResolutionX");

    lua_pushinteger(Game::Lua::State, Size.Y);
    lua_setfield(Game::Lua::State, TableIndex, "ResolutionY");

    const lua_Number AspectRatio =
        static_cast<lua_Number>(Size.X) / static_cast<lua_Number>(Size.Y);

    lua_pushnumber(Game::Lua::State, AspectRatio);
    lua_setfield(Game::Lua::State, TableIndex, "AspectRatio");

    lua_pushnumber(Game::Lua::State, static_cast<lua_Number>(1) / AspectRatio);
    lua_setfield(Game::Lua::State, TableIndex, "AspectRatioReciprocal");

    Math::Mat4* OrthographicProjection = static_cast<Math::Mat4*>(
        lua_newuserdata(Game::Lua::State, sizeof(Math::Mat4)));

    luaL_getmetatable(Game::Lua::State, "Mat4");
    lua_setmetatable(Game::Lua::State, -2);

    *OrthographicProjection = Math::Transform::Orthographic(
        -AspectRatio, AspectRatio, -1.f, 1.f);  //, -1.f, 1.f)
}

void Game::Lua::CLibraries::Graphics::etc::
    _lua_graphics_WindowResizedEvent_updateLuaSize(const Math::IVector2& Size) {
    Game::Lua::GameTable.PushKey(Game::Lua::State, "Graphics");

    _lua_graphics_updateScreenSize(-2, Size);

    lua_pop(Game::Lua::State, 1);
}

#endif