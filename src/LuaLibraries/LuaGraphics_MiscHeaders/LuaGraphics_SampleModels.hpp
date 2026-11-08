#pragma once

#include "../../../include/VM/lua.h"

#include "../../FunctionHeaders/LuaHelper.hpp"

namespace Game::Lua::CLibraries::Graphics::SampleModels {
    namespace etc {
        void _CreateSampleModelEntry(lua_State* State,
                                     const void* Verticies,
                                     size_t VertexCount,
                                     size_t VertexSize,
                                     const void* Indices,
                                     size_t IndicesCount,
                                     size_t IndiceSize,
                                     const char* ModelName);
    }
    
    inline void Init(lua_State* State,
                     LuaHelper::StackTableReference& GraphicsTable);
}

void Game::Lua::CLibraries::Graphics::SampleModels::Init(
    lua_State* State,
    LuaHelper::StackTableReference& GraphicsTable) {
    constexpr static float FlatQuadModel[] = {-1.f, -1.f, 0.f, 0.f, -1.f, 1.f,
                                              0.f,  1.f,  1.f, 1.f, 1.f,  1.f,
                                              1.f,  -1.f, 1.f, 0.f};
    constexpr static Uint8 FlatQuadModelIndices[] = {0, 1, 2, 2, 3, 0};

    lua_createtable(State, 0, 4);

    etc::_CreateSampleModelEntry(State, FlatQuadModel,
                              sizeof(FlatQuadModel) / sizeof(float),
                              sizeof(float), FlatQuadModelIndices,
                              sizeof(FlatQuadModelIndices) / sizeof(Uint8),
                              sizeof(Uint8), "FlatQuad");

    lua_setfield(State, GraphicsTable.GetStackIndex(), "SampleModels");
}