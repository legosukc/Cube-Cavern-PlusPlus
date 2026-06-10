#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <lua-5.5.0/lua.hpp>

#include "LuaBuffer.hpp"

#include <SDL3/SDL_stdinc.h>


namespace Game::Lua::CLibraries::ModelLoader {
	inline void Init(lua_State* State);

	static int LoadFile(lua_State* State);
}

void Game::Lua::CLibraries::ModelLoader::Init(lua_State* State) {

    lua_createtable(State, 0, 2);
    
    lua_pushcfunction(State, CLibraries::ModelLoader::LoadFile);
    lua_setfield(State, -2, "LoadFile");

    lua_setglobal(State, "ModelLoader");
}


int Game::Lua::CLibraries::ModelLoader::LoadFile(lua_State* State) {
    
    using BufferUD = CLibraries::Buffer::Classes::Buffer;
    struct Vertex {
        aiVector3D Position;
        aiVector3D Normal;
        aiVector2D TexCoords;
    };

    size_t VertexIndex;
    aiMesh* Mesh;
    size_t MeshIndex;
    BufferUD* LuaBuffer;
    /*
    Assimp::Importer importer;
    const aiScene* scene;

    scene = importer.ReadFile(luaL_checkstring(State, 1), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
    
    // if is Not Zero
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        luaL_error(State, "Error while calling function 'ModelLoader.LoadFile'. Assimp error: ", importer.GetErrorString());
        return 0;
    }*/

    lua_createtable(State, 0, 4);
    /*
    LuaBuffer = static_cast<BufferUD*>(lua_newuserdata(State, sizeof(BufferUD)));
    LuaBuffer->Size = 0;
    for (MeshIndex = 0; MeshIndex < scene->mNumMeshes; ++MeshIndex) {
        LuaBuffer->Size += scene->mMeshes[MeshIndex]->mNumVertices * sizeof(Vertex);
    }
    LuaBuffer->Data = std::malloc(LuaBuffer->Size);
  
    for (MeshIndex = 0; MeshIndex < scene->mNumMeshes; ++MeshIndex) {

        Mesh = scene->mMeshes[MeshIndex];

        for (VertexIndex = 0; VertexIndex < Mesh->mNumVertices; ++VertexIndex) {
            static_cast<Vertex*>(LuaBuffer->Data)[VertexIndex] = Vertex{
                Mesh->mVertices[VertexIndex],
                Mesh->mNormals[VertexIndex],
                aiVector2D(Mesh->mTextureCoords[VertexIndex]->x, Mesh->mTextureCoords[VertexIndex]->y),
            };
        }
    }*/

    lua_setfield(State, -2, "VertexData");

    /*
    LuaBuffer = static_cast<BufferUD*>(lua_newuserdata(State, sizeof(BufferUD)));
    LuaBuffer->Size = 0;
    for (MeshIndex = 0; MeshIndex < scene->mNumMeshes; ++MeshIndex) {
        LuaBuffer->Size += scene->mMeshes[MeshIndex]->mNumIndices * sizeof(unsigned int);
    }
    char* IndexDataPtr = std::malloc(LuaBuffer->Size);
    LuaBuffer->Data = IndexDataPtr;

    for (MeshIndex = 0; MeshIndex < scene->mNumMeshes; ++MeshIndex) {

        Mesh = scene->mMeshes[MeshIndex];
        std::memcpy(IndexDataPtr, Mesh->mIndices, Mesh->mNumIndices * sizeof(unsigned int));
        IndexDataPtr += Mesh->mNumIndices * sizeof(unsigned int);
    }*/

    lua_setfield(State, -2, "IndexData");

    lua_pushinteger(State, sizeof(Vertex));
    lua_setfield(State, -2, "StrideSize");

	return 1;
}