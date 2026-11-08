#include "LuaGraphics_SampleModels.hpp"

#include <string.h>
#include "../../../include/VM/lua.h"

void Game::Lua::CLibraries::Graphics::SampleModels::etc::_CreateSampleModelEntry(
    lua_State* State,
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

    memcpy(lua_newbuffer(State, VertexCount * VertexSize), Verticies,
                VertexCount * VertexSize);
    lua_setfield(State, -2, "Vertices");

    memcpy(lua_newbuffer(State, IndicesCount * IndiceSize), Indices,
                IndicesCount * IndiceSize);
    lua_setfield(State, -2, "Indices");

    lua_setfield(State, -2, ModelName);
}