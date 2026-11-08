#include "LuaGraphics_GLObjectBase.hpp"

#ifdef SDL_PLATFORM_VITA
#include <GLES2/gl2.h>
#else
#include <SDL3/SDL_opengl.h>
#endif

#include "../../../include/VM/lua.h"

#include "../../FunctionHeaders/LuaHelper.hpp"

LuaHelper::StackTableReference
    Game::Lua::CLibraries::Graphics::Classes::GLObjectBase::InitMetatable(
        lua_State* State,
        const char* MetatableName) {
    LuaHelper::StackTableReference Metatable(State, MetatableName);

    Metatable.SetKey(State, GLObjectBase::__gc, "__gc");

    Metatable.PushReference(State);
    lua_setfield(State, Metatable.GetStackIndex(), "__index");

    return Metatable;
}

int Game::Lua::CLibraries::Graphics::Classes::GLObjectBase::__gc(
    lua_State* State) {
    return 0;
}