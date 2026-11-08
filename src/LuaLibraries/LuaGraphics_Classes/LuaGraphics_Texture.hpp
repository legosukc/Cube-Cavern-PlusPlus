#pragma once

#include "../../define.h"

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
        struct Texture {
            static constexpr char MetatableName[] = "Texture";

            GLuint TextureObject;

            static int Bind(lua_State* State);

            static int __gc(lua_State* State);
        };
    }

    namespace Texture {

        constexpr char LibraryName[] = "Texture";

        inline void Init(lua_State* State,
                         LuaHelper::StackTableReference& GraphicsTable);

        int Unbind(lua_State* State);
        int SetActiveTexture(lua_State* State);

        int SetFilteringUpscale(lua_State* State);
        int SetFilteringDownscale(lua_State* State);

        int SetWrappingHorizontal(lua_State* State);
        int SetWrappingVertical(lua_State* State);

        int SetTexImage2D(lua_State* State);
        int Load2DImageFromFile(lua_State* State);

        int __new(lua_State* State);
    }
}

void Game::Lua::CLibraries::Graphics::Texture::Init(
    lua_State* State,
    LuaHelper::StackTableReference& GraphicsTable) {
    LuaHelper::StackTableReference TextureMetatable(
        State, CLibraries::Graphics::Classes::Texture::MetatableName);

    TextureMetatable.SetKeyClosure(
        State, CLibraries::Graphics::Classes::Texture::Bind, "Bind");

    TextureMetatable.SetKeyClosure(
        State, CLibraries::Graphics::Classes::Texture::__gc, "__gc");

    TextureMetatable.PushReference(State);
    lua_setfield(State, TextureMetatable.GetStackIndex(), "__index");

    lua_settop(State, TextureMetatable.GetStackIndex() - 1);

    LuaHelper::StackTableReference TextureTable(State, 0, 1);

    TextureTable.SetKeyClosure(State, CLibraries::Graphics::Texture::__new,
                               "new");

    TextureTable.SetKeyClosure(
        State, CLibraries::Graphics::Texture::SetFilteringDownscale,
        "SetFilteringDownscale");

    TextureTable.SetKeyClosure(
        State, CLibraries::Graphics::Texture::SetFilteringUpscale,
        "SetFilteringUpscale");

    TextureTable.SetKeyClosure(
        State, CLibraries::Graphics::Texture::SetWrappingHorizontal,
        "SetWrappingHorizontal");

    TextureTable.SetKeyClosure(
        State, CLibraries::Graphics::Texture::SetWrappingVertical,
        "SetWrappingVertical");

    TextureTable.SetKeyClosure(State,
                               CLibraries::Graphics::Texture::SetActiveTexture,
                               "SetActiveTexture");

    TextureTable.SetKeyClosure(
        State, CLibraries::Graphics::Texture::Load2DImageFromFile,
        "Load2DImageFromFile");

    TextureTable.SetKeyClosure(
        State, CLibraries::Graphics::Texture::SetTexImage2D, "SetTexImage2D");

    lua_setfield(State, GraphicsTable.GetStackIndex(),
                 CLibraries::Graphics::Texture::LibraryName);
}