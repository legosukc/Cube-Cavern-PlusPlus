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

#include "../../Client/Graphics/Classes/VertexArray.hpp"

int Game::Lua::CLibraries::Graphics::VertexArray::__new(lua_State* State) {
    Game::Graphics::Classes::VertexArray** VertexArrayUD =
        reinterpret_cast<Game::Graphics::Classes::VertexArray**>(
            lua_newuserdata(State,
                            sizeof(Game::Graphics::Classes::VertexArray*)));
    *VertexArrayUD = Game::Graphics::VertexArray::Create();
    luaL_getmetatable(State, "VertexArray");
    lua_setmetatable(State, -2);
    return 1;
}

int Game::Lua::CLibraries::Graphics::Classes::VertexArray::Bind(
    lua_State* State) {
    (*reinterpret_cast<Game::Graphics::Classes::VertexArray**>(
         LuaHelper::CheckMetatable(State, 1, lua_upvalueindex(1))))
        ->Bind();
    return 0;
}

int Game::Lua::CLibraries::Graphics::VertexArray::AddFloatAttribute(
    lua_State* State) {
    const lua_Integer Count = luaL_checkinteger(State, 2);
    if (Count < 1 || Count > 4) {
        luaL_error(State,
                   "Invalid range on 2nd parameter 'Count', expected [1-4].");
    }

    Game::Graphics::VertexArray::AddFloatAttribute(
        static_cast<int>(luaL_checkinteger(State, 1)),
        static_cast<size_t>(Count),
        static_cast<size_t>(luaL_checkinteger(State, 3)));
    return 0;
}

int Game::Lua::CLibraries::Graphics::VertexArray::AddIntAttribute(
    lua_State* State) {
    const lua_Integer Count = luaL_checkinteger(State, 2);
    if (Count < 1 || Count > 4) {
        luaL_error(State,
                   "Invalid range on 2nd parameter 'Count', expected [1-4].");
    }

    Game::Graphics::VertexArray::AddIntAttribute(
        static_cast<int>(luaL_checkinteger(State, 1)),
        static_cast<size_t>(Count),
        static_cast<size_t>(luaL_checkinteger(State, 3)));
    return 0;
}

int Game::Lua::CLibraries::Graphics::VertexArray::Unbind(lua_State* State) {
    Game::Graphics::VertexArray::Unbind();
    return 0;
}

int Game::Lua::CLibraries::Graphics::Classes::VertexArray::__gc(
    lua_State* State) {
    delete *reinterpret_cast<Classes::VertexArray**>(
        luaL_checkudata(State, 1, "VertexArray"));
    return 0;
}
