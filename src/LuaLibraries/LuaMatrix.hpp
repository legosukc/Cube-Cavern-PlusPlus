#pragma once

#include "../../include/VM/lua.h"

#include "../MathClasses/Matrix.hpp"

#include "../FunctionHeaders/LuaHelper.hpp"

namespace Game::Lua::CLibraries::Matrix {

    inline void Init(lua_State* State);

    template<class MatrixType>
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

    template <class MatrixType>
    void SetupMatrix(lua_State* State,
                     const char* VectorName,
                     const char* MatrixName);
}

template <class MatrixType>
int Game::Lua::CLibraries::Matrix::TemplatedFunctions<MatrixType>::__new(
    lua_State* State) {
    typename MatrixType::ColumnType* Row1;
    MatrixType* NewMatrix;

    NewMatrix =
        static_cast<MatrixType*>(lua_newuserdata(State, sizeof(MatrixType)));

    lua_pushvalue(State, lua_upvalueindex(1));
    lua_setmetatable(State, -2);

    if (lua_gettop(State) == 2) {
        *NewMatrix = MatrixType(
            LuaHelper::ToTypename<typename MatrixType::ComponentType>(State,
                                                                      1));
        return 1;
    }
    lua_insert(State, 1);

    if (Row1 = static_cast<typename MatrixType::ColumnType*>(
            LuaHelper::TestMetatable(State, 1, lua_upvalueindex(1)));
        Row1 == NULL) {
        lua_settop(State, (MatrixType::RowCount * MatrixType::ColumnCount) + 1);

        for (int i = 2; i <= MatrixType::RowCount * MatrixType::ColumnCount;
             ++i) {
            reinterpret_cast<typename MatrixType::ColumnType::ComponentType*>(
                NewMatrix)[i - 2] = luaL_optnumber(State, i, 0.0);
        }

    } else {
        lua_settop(State, MatrixType::ColumnCount + 1);

        (*NewMatrix)[0] = *Row1;
        for (int i = 3; i <= MatrixType::ColumnCount; ++i) {
            (*NewMatrix)[i - 2] =
                *static_cast<typename MatrixType::ColumnType*>(
                    LuaHelper::TestMetatable(State, i, lua_upvalueindex(1)));
        }
    }

    lua_settop(State, 1);
    return 1;
}

template <class MatrixType>
int Game::Lua::CLibraries::Matrix::TemplatedFunctions<MatrixType>::__tostring(
    lua_State* State) {
    const MatrixType* MatrixUD = static_cast<MatrixType*>(
        LuaHelper::CheckMetatable(State, 1, lua_upvalueindex(1)));

    for (int Column = 0; Column < MatrixType::ColumnCount; ++Column) {
        for (int Row = 0; Row < MatrixType::RowCount; ++Row) {
            LuaHelper::Push<typename MatrixType::ComponentType>(
                State, MatrixUD->IndexElement(Row * Column));
        }
        lua_pushstring(State, "\n");
    }
    lua_concat(State, MatrixType::RowCount * MatrixType::ColumnCount);
    /*
    LuaHelper::Push<typename
    MatrixType::ColumnType::ComponentType>(State,
    (*MatrixUD)[MatrixType::ColumnType]); for (int i =
    VectorType::ComponentCount - 1; i > 0; --i) {

            //std::cout << i << std::endl;
            lua_pushliteral(State, ", ");
            LuaHelper::Push<typename
    MatrixType::ColumnType::ComponentType>(State, (*Vector)[i]);
    }
    lua_concat(State, VectorType::ComponentCount);

    lua_rotate(State, 1, 1);*/

    return 1;
}

template <class MatrixType>
int Game::Lua::CLibraries::Matrix::TemplatedFunctions<MatrixType>::__index(
    lua_State* State) {
    lua_Integer Index;
    MatrixType* MatrixUD;

    MatrixUD = static_cast<MatrixType*>(
        LuaHelper::CheckMetatable(State, 1, lua_upvalueindex(1)));
    Index = luaL_checkinteger(State, 2);

    unlikely_branch if (Index < 0 || Index > MatrixType::ColumnCount - 1) {
        lua_getfield(State, lua_upvalueindex(1), "__name");
        luaL_error(State, "Cannot index ", lua_tostring(State, -1),
                   ": Numerical index out of range: (", Index, ").");
    }

    LuaHelper::Push<typename MatrixType::ColumnType>(State, (*MatrixUD)[Index]);
    return 1;
}

template <class MatrixType>
int Game::Lua::CLibraries::Matrix::TemplatedFunctions<MatrixType>::__newindex(
    lua_State* State) {
    lua_Integer Index;
    MatrixType* MatrixUD;

    MatrixUD = static_cast<MatrixType*>(
        LuaHelper::CheckMetatable(State, 1, lua_upvalueindex(1)));
    Index = luaL_checkinteger(State, 2);

    unlikely_branch if (Index < 0 || Index > MatrixType::ColumnCount - 1) {
        lua_getfield(State, lua_upvalueindex(1), "__name");
        luaL_error(State, "Cannot index ", lua_tostring(State, -1),
                   ": Numerical index out of range: (", Index, ").");
    }

    (*MatrixUD)[Index] = *static_cast<typename MatrixType::ColumnType*>(
        LuaHelper::CheckMetatable(State, 3, lua_upvalueindex(2)));
    return 0;
}

template <class MatrixType>
int Game::Lua::CLibraries::Matrix::TemplatedFunctions<MatrixType>::__mul(
    lua_State* State) {
    MatrixType *Result, *B;

    Result =
        static_cast<MatrixType*>(lua_newuserdata(State, sizeof(MatrixType)));

    *Result = *static_cast<MatrixType*>(
                  LuaHelper::CheckMetatable(State, 1, lua_upvalueindex(1))) *
              *static_cast<MatrixType*>(
                  LuaHelper::CheckMetatable(State, 2, lua_upvalueindex(1)));
    return 1;
}

template <class MatrixType>
int Game::Lua::CLibraries::Matrix::TemplatedFunctions<MatrixType>::__lerp(
    lua_State* State) {
    MatrixType *Result, *B;

    Result =
        static_cast<MatrixType*>(lua_newuserdata(State, sizeof(MatrixType)));

    *Result = static_cast<MatrixType*>(
                  LuaHelper::CheckMetatable(State, 1, lua_upvalueindex(1)))
                  ->Lerp(*static_cast<MatrixType*>(LuaHelper::CheckMetatable(
                      State, 2, lua_upvalueindex(1))));
    return 1;
}

template <class MatrixType>
int Game::Lua::CLibraries::Matrix::TemplatedFunctions<MatrixType>::__eq(
    lua_State* State) {
    const MatrixType *A, *B;

    A = static_cast<MatrixType*>(
        LuaHelper::TestMetatable(State, 1, lua_upvalueindex(1)));
    B = static_cast<MatrixType*>(
        LuaHelper::TestMetatable(State, 2, lua_upvalueindex(1)));

    lua_pushboolean(State, !(A == NULL || B == NULL) && A->operator==(*B));
    return 1;
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

    LibraryTable.SetKeyClosure(State, TemplatedFunctions<MatrixType>::__new,
                               "new", 1);
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

    lua_pop(State, 1);
}