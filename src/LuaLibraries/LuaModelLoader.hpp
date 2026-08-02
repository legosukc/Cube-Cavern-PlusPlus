#ifndef LUA_MODELLOADER_H
#define LUA_MODELLOADER_H

// TODO

#include <iostream>

#include <SDL3/SDL_mouse.h>
#include <assimp/mesh.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>

#include "../../include/VM/lua.h"
#include "../../include/VM/lualib.h"

#include <SDL3/SDL_stdinc.h>
#include <assimp/vector2.h>
#include <assimp/vector3.h>

namespace Game::Lua::CLibraries::ModelLoader {
    inline void Init(lua_State* State);

    static int LoadFile(lua_State* State);
}

void Game::Lua::CLibraries::ModelLoader::Init(lua_State* State) {
    lua_createtable(State, 0, 2);

    lua_pushcfunction(State, CLibraries::ModelLoader::LoadFile,
                      "ModelLoader.LoadFile");
    lua_setfield(State, -2, "LoadFile");

    lua_setglobal(State, "ModelLoader");
}

int Game::Lua::CLibraries::ModelLoader::LoadFile(lua_State* State) {
    // using BufferUD = CLibraries::Buffer::Classes::Buffer;
    struct Vertex {
        aiVector3D Position;
        aiVector3D Normal;
        aiVector2D TexCoords;
    };

    Assimp::Importer importer;
    const aiScene* scene =
        importer.ReadFile(luaL_checkstring(State, 1),
                          aiProcess_Triangulate | aiProcess_GenSmoothNormals |
                              aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
        !scene->mRootNode) {
        luaL_error(State,
                   "Error while calling function 'ModelLoader.LoadFile'. "
                   "Assimp error: ",
                   importer.GetErrorString());
        return 0;
    }

    //*scene;
    lua_createtable(State, 0, 4);

    size_t BufferSize = 0;
    size_t MeshIndex;
    for (MeshIndex = 0; MeshIndex < scene->mNumMeshes; ++MeshIndex) {
        BufferSize += scene->mMeshes[MeshIndex]->mNumVertices;
    }

    lua_pushnumber(State, BufferSize);
    lua_setfield(State, -2, "VerticeCount");
    BufferSize *= sizeof(Vertex);

    char* LuaBuffer = (char*)lua_newbuffer(State, BufferSize);
    if (LuaBuffer == NULL) {
        luaL_error(State,
                   "C API Error: Failed to allocate 'VertexData' buffer. Maybe "
                   "you're out of memory?");
        return 0;
    }

    aiMesh* Mesh;
    for (MeshIndex = 0; MeshIndex < scene->mNumMeshes; ++MeshIndex) {
        Mesh = scene->mMeshes[MeshIndex];

        for (size_t VertexIndex = 0; VertexIndex < Mesh->mNumVertices;
             ++VertexIndex) {
            reinterpret_cast<Vertex*>(LuaBuffer)->Position =
                Mesh->mVertices ? Mesh->mVertices[VertexIndex]
                                : aiVector3D(0.f, 0.f, 0.f);

            reinterpret_cast<Vertex*>(LuaBuffer)->Normal =
                Mesh->mNormals ? Mesh->mNormals[VertexIndex]
                               : aiVector3D(0.f, 0.f, 0.f);

            reinterpret_cast<Vertex*>(LuaBuffer)->TexCoords =
                Mesh->mTextureCoords[0]
                    ? aiVector2D(Mesh->mTextureCoords[0][VertexIndex].x,
                                 Mesh->mTextureCoords[0][VertexIndex].y)
                    : aiVector2D(0.f, 0.f);
        };
        LuaBuffer += sizeof(Vertex);
    }

    lua_setfield(State, -2, "VertexData");

    BufferSize = 0;
    for (MeshIndex = 0; MeshIndex < scene->mNumMeshes; ++MeshIndex) {
        Mesh = scene->mMeshes[MeshIndex];
        for (size_t FaceIndex = 0; FaceIndex < Mesh->mNumFaces; ++FaceIndex) {
            BufferSize +=
                Mesh->mFaces[FaceIndex].mNumIndices;
        }
    }

    lua_pushnumber(State, BufferSize);
    lua_setfield(State, -2, "IndexCount");

    BufferSize *= sizeof(unsigned int);

    LuaBuffer = (char*)lua_newbuffer(State, BufferSize);
    if (LuaBuffer == NULL) {
        luaL_error(State,
                   "C API Error: Failed to allocate 'IndexData' buffer. Maybe "
                   "you're out of memory?");
        return 0;
    }

    for (MeshIndex = 0; MeshIndex < scene->mNumMeshes; ++MeshIndex) {
        Mesh = scene->mMeshes[MeshIndex];
        for (size_t FaceIndex = 0; FaceIndex < Mesh->mNumFaces; ++FaceIndex) {
            std::memcpy(
                LuaBuffer, Mesh->mFaces[FaceIndex].mIndices,
                Mesh->mFaces[FaceIndex].mNumIndices * sizeof(unsigned int));
            LuaBuffer +=
                Mesh->mFaces[FaceIndex].mNumIndices * sizeof(unsigned int);
        }
    }

    lua_setfield(State, -2, "IndexData");

    lua_pushinteger(State, sizeof(Vertex));
    lua_setfield(State, -2, "StrideSize");

    return 1;
}

#endif