#ifndef LUAGRAPHICS_SAMPLEMODELS
#define LUAGRAPHICS_SAMPLEMODELS

#include <cstring>
#include "../../../include/VM/lua.h"

#include "../../FunctionHeaders/LuaHelper.hpp"

namespace Game::Lua::CLibraries::Graphics::SampleModels {
    inline void Init(lua_State* State,
                     LuaHelper::StackTableReference& GraphicsTable);
}

namespace {
    static void _CreateSampleModelEntry(lua_State* State,
                                        const void* Verticies,
                                        size_t VertexCount,
                                        size_t VertexSize,
                                        const void* Indices,
                                        size_t IndicesCount,
                                        size_t IndiceSize,
                                        const char* ModelName) {
        lua_createtable(State, 0, 4);

        lua_pushinteger(State, VertexCount);
        lua_setfield(State, -2, "VertexCount");

        lua_pushinteger(State, IndicesCount);
        lua_setfield(State, -2, "IndiceCount");

        std::memcpy(lua_newbuffer(State, VertexCount * VertexSize), Verticies,
                    VertexCount * VertexSize);
        lua_setfield(State, -2, "Vertices");

        std::memcpy(lua_newbuffer(State, IndicesCount * IndiceSize), Indices,
                    IndicesCount * IndiceSize);
        lua_setfield(State, -2, "Indices");

        lua_setfield(State, -2, ModelName);
    }
}

void Game::Lua::CLibraries::Graphics::SampleModels::Init(
    lua_State* State,
    LuaHelper::StackTableReference& GraphicsTable) {
    constexpr static float FlatQuadModel[] = {-1.f, -1.f, 0.f, 0.f, -1.f, 1.f,
                                              0.f,  1.f,  1.f, 1.f, 1.f,  1.f,
                                              1.f,  -1.f, 1.f, 0.f};
    constexpr static Uint8 FlatQuadModelIndices[] = {0, 1, 2, 2, 3, 0};

    lua_createtable(State, 0, 4);

    ::_CreateSampleModelEntry(State, FlatQuadModel,
                              sizeof(FlatQuadModel) / sizeof(float),
                              sizeof(float), FlatQuadModelIndices,
                              sizeof(FlatQuadModelIndices) / sizeof(Uint8),
                              sizeof(Uint8), "FlatQuad");

    lua_setfield(State, GraphicsTable.GetStackIndex(), "SampleModels");
}

#endif