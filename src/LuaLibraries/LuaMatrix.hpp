#pragma once

#include "../../include/VM/lua.h"

#include "../MathClasses/Matrix.hpp"

#include "../FunctionHeaders/LuaHelper.hpp"

namespace Game::Lua::CLibraries::Matrix {

    inline void Init(lua_State* State);

    template <class MatrixType>
    struct TemplatedFunctions {
        static int __new(lua_State* State);
        static int __tostring(lua_State* State);
        static int __index(lua_State* State);
        static int __newindex(lua_State* State);

        static int __mul(lua_State* State);
        static int __lerp(lua_State* State);
        static int __eq(lua_State* State);
    };

    int _LookAt(lua_State* State);
    int _Perspective(lua_State* State);
    int _Orthographic(lua_State* State);
    int __Translate(lua_State* State);
    int __RotateAxis(lua_State* State);
    int __Rotate(lua_State* State);

    template <class MatrixType>
    inline void SetupMatrix(lua_State* State,
                            const char* VectorName,
                            const char* MatrixName);
}

template <class MatrixType>
void Game::Lua::CLibraries::Matrix::SetupMatrix(lua_State* State,
                                                const char* VectorName,
                                                const char* MatrixName) {
    LuaHelper::StackTableReference LibraryTable, Metatable;

    LibraryTable = LuaHelper::StackTableReference(State, 0, 1);
    Metatable = LuaHelper::StackTableReference(State, MatrixName);

    for (Uint8 i = 0; i < 7; ++i) {
        // Metatable.PushReference(State);
    }

    // Metatable.PushReference(State);
    // lua_copy(State, Metatable.GetStackIndex() + 1,
    // Metatable.GetStackIndex() + 6);

    Metatable.PushReference(State);
    Metatable.SetKeyClosure(State, TemplatedFunctions<MatrixType>::__tostring,
                            "__tostring", 1);

    Metatable.PushReference(State);
    Metatable.SetKeyClosure(State, TemplatedFunctions<MatrixType>::__index,
                            "__index", 1);

    Metatable.PushReference(State);
    luaL_getmetatable(State, VectorName);
    Metatable.SetKeyClosure(State, TemplatedFunctions<MatrixType>::__newindex,
                            "__newindex", 2);

    Metatable.PushReference(State);
    Metatable.SetKeyClosure(State, TemplatedFunctions<MatrixType>::__mul,
                            "__mul", 1);

    Metatable.PushReference(State);
    Metatable.SetKeyClosure(State, TemplatedFunctions<MatrixType>::__eq, "__eq",
                            1);

    // TODO: follow passed template
    lua_pushliteral(State, "Vector4");
    LibraryTable.SetKeyClosure(State, TemplatedFunctions<MatrixType>::__new,
                               "new", 2);
    LibraryTable.SetKey<lua_Integer>(State, sizeof(MatrixType), "ByteSize");

    lua_setglobal(State, MatrixName);
}

void Game::Lua::CLibraries::Matrix::Init(lua_State* State) {
    //::SetupMatrix<Math::Mat2>(State, "Vector2", "Mat2");
    //::SetupMatrix<Math::Mat3>(State, "Vector3", "Mat3");

    Game::Lua::CLibraries::Matrix::SetupMatrix<Math::Mat4>(State, "Vector4",
                                                           "Mat4");

    lua_getglobal(State, "Mat4");

    lua_pushcfunction(State, Game::Lua::CLibraries::Matrix::_LookAt,
                      "Mat4.LookAt");
    lua_setfield(State, -2, "LookAt");

    lua_pushcfunction(State, Game::Lua::CLibraries::Matrix::_Perspective,
                      "Mat4.Perspective");
    lua_setfield(State, -2, "Perspective");

    lua_pushcfunction(State, Game::Lua::CLibraries::Matrix::_Orthographic,
                      "Mat4.Orthographic");
    lua_setfield(State, -2, "Orthographic");

    lua_pushcfunction(State, Game::Lua::CLibraries::Matrix::__Translate,
                      "Mat4.Translate");
    lua_setfield(State, -2, "Translate");

    lua_pushcfunction(State, Game::Lua::CLibraries::Matrix::__RotateAxis,
                      "Mat4.RotateAxis");
    lua_setfield(State, -2, "RotateAxis");

    lua_pushcfunction(State, Game::Lua::CLibraries::Matrix::__Rotate,
                      "Mat4.Rotate");
    lua_setfield(State, -2, "Rotate");

    lua_pop(State, 1);
}