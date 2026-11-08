#pragma once

#include <SDL3/SDL_mouse.h>
#include <SDL3/SDL_stdinc.h>

#include <assimp/mesh.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/vector2.h>
#include <assimp/vector3.h>
#include <assimp/Importer.hpp>

#include "../../include/VM/lua.h"

namespace Game::Lua::CLibraries::ModelLoader {
    inline void Init(lua_State* State);

    int LoadFile(lua_State* State);
}

void Game::Lua::CLibraries::ModelLoader::Init(lua_State* State) {
    lua_createtable(State, 0, 2);

    lua_pushcfunction(State, CLibraries::ModelLoader::LoadFile,
                      "ModelLoader.LoadFile");
    lua_setfield(State, -2, "LoadFile");

    lua_setglobal(State, "ModelLoader");
}