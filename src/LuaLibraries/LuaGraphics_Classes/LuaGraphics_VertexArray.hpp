#pragma once

#include "LuaGraphics_GLObjectBase.hpp"

#ifdef SDL_PLATFORM_VITA
#include <GLES2/gl2.h>
#include "../../GLES2_typedefs.h"
#else
#include <SDL3/SDL_opengl.h>
#include <SDL3/SDL_opengl_glext.h>
#endif

#include "../../../include/VM/lua.h"

#include "../../FunctionHeaders/LuaHelper.hpp"

namespace Game::Lua::CLibraries::Graphics {

    namespace Classes {
        struct VertexArray
            : Game::Lua::CLibraries::Graphics::Classes::GLObjectBase {
            static int Bind(lua_State* State);

            // static int __eq(lua_State* State);
            static int __gc(lua_State* State);
        };
    }

    namespace VertexArray {
        inline void Init(lua_State* State,
                         LuaHelper::StackTableReference& GraphicsTable);

        int __new(lua_State* State);

        int AddFloatAttribute(lua_State* State);
        int AddIntAttribute(lua_State* State);

        int Unbind(lua_State* State);
    }
}

void Game::Lua::CLibraries::Graphics::VertexArray::Init(
    lua_State* State,
    LuaHelper::StackTableReference& GraphicsTable) {
    LuaHelper::StackTableReference VertexArrayMetatable(State, "VertexArray");

    VertexArrayMetatable.PushReference(State);
    VertexArrayMetatable.SetKeyClosure(State, Classes::VertexArray::Bind,
                                       "Bind", 1);

    VertexArrayMetatable.SetKeyClosure(State, Classes::VertexArray::__gc,
                                       "__gc");
    // VertexArrayMetatable.SetKeyClosure(State, Classes::VertexArray::__eq,
    // "__eq");

    VertexArrayMetatable.PushReference(State);
    lua_setfield(State, VertexArrayMetatable.GetStackIndex(), "__index");

    lua_settop(State, VertexArrayMetatable.GetStackIndex() - 1);

    LuaHelper::StackTableReference VertexArrayTable(State, 0,
                                                    4);  // OpenGL.VertexArray

    VertexArrayTable.SetKeyClosure(State, VertexArray::__new, "new");

    VertexArrayTable.SetKeyClosure(State, VertexArray::AddFloatAttribute,
                                   "AddFloatAttribute");
    VertexArrayTable.SetKeyClosure(State, VertexArray::AddIntAttribute,
                                   "AddIntAttribute");

    VertexArrayTable.SetKeyClosure(State, VertexArray::Unbind, "Unbind");

    lua_setfield(State, GraphicsTable.GetStackIndex(), "VertexArray");
}