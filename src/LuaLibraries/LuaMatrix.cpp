#include "LuaMatrix.hpp"

#include "../../include/VM/lua.h"
#include "../../include/VM/lualib.h"

#include "../FunctionHeaders/LuaHelper.hpp"

#include "../FunctionHeaders/MathTransform.hpp"
#include "../MathClasses/Matrix.hpp"

/*
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

    LibraryTable.SetKeyClosure(State, TemplatedFunctions<MatrixType>::__new,
                               "new", 1);
    LibraryTable.SetKey<lua_Integer>(State, sizeof(MatrixType), "ByteSize");

    lua_setglobal(State, MatrixName);
}*/

int Game::Lua::CLibraries::Matrix::_LookAt(lua_State* State) {
    Math::Mat4* Result =
        static_cast<Math::Mat4*>(lua_newuserdata(State, sizeof(Math::Mat4)));
    luaL_getmetatable(State, "Mat4");
    lua_setmetatable(State, -2);

    *Result =
        Math::Transform::LookAt(*static_cast<Math::Vector3*>(luaL_checkudata(
                                    State, 1, "Vector3")),  // Eye
                                *static_cast<Math::Vector3*>(luaL_checkudata(
                                    State, 2, "Vector3")),  // Center
                                *static_cast<Math::Vector3*>(
                                    luaL_checkudata(State, 3, "Vector3"))  // Up
        );

    return 1;
}

int Game::Lua::CLibraries::Matrix::_Perspective(lua_State* State) {
    Math::Mat4* Result =
        static_cast<Math::Mat4*>(lua_newuserdata(State, sizeof(Math::Mat4)));

    luaL_getmetatable(State, "Mat4");
    lua_setmetatable(State, -2);

    *Result = Math::Transform::Perspective(
        static_cast<float>(luaL_checknumber(State, 1)),  // FOV
        static_cast<float>(luaL_checknumber(State, 2)),  // Aspect Ratio
        static_cast<float>(luaL_checknumber(State, 3)),  // zNear
        static_cast<float>(luaL_checknumber(State, 4))   // zFar
        //*static_cast<zKevin*>(luaL_checkudata(State, !7, "zKevin"))
    );

    return 1;
}

int Game::Lua::CLibraries::Matrix::_Orthographic(lua_State* State) {
    Math::Mat4* Result =
        static_cast<Math::Mat4*>(lua_newuserdata(State, sizeof(Math::Mat4)));

    luaL_getmetatable(State, "Mat4");
    lua_setmetatable(State, -2);

    *Result = Math::Transform::Orthographic(
        static_cast<float>(luaL_checknumber(State, 1)),  // Left
        static_cast<float>(luaL_checknumber(State, 2)),  // Right
        static_cast<float>(luaL_checknumber(State, 3)),  // Bottom
        static_cast<float>(luaL_checknumber(State, 4))   // Top
    );

    return 1;
}

int Game::Lua::CLibraries::Matrix::__Translate(lua_State* State) {
    Math::Mat4* Result =
        static_cast<Math::Mat4*>(lua_newuserdata(State, sizeof(Math::Mat4)));

    luaL_getmetatable(State, "Mat4");
    lua_setmetatable(State, -2);

    *Result = Math::Transform::Translate(
        *static_cast<Math::Mat4*>(luaL_checkudata(State, 1, "Mat4")),
        *static_cast<Math::Vector3*>(luaL_checkudata(State, 2, "Vector3")));

    return 1;
}

/*
template void Game::Lua::CLibraries::Matrix::SetupMatrix<Math::Mat2>(
    lua_State* State,
    const char* VectorName,
    const char* MatrixName);

template void Game::Lua::CLibraries::Matrix::SetupMatrix<Math::Mat3>(
    lua_State* State,
    const char* VectorName,
    const char* MatrixName);

template void Game::Lua::CLibraries::Matrix::SetupMatrix<Math::Mat4>(
    lua_State* State,
    const char* VectorName,
    const char* MatrixName);*/