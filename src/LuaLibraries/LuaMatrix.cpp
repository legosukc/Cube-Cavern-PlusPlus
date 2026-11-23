#include "LuaMatrix.hpp"

#include "../../include/VM/lua.h"
#include "../../include/VM/lualib.h"

#include "../FunctionHeaders/LuaHelper.hpp"

#include "../FunctionHeaders/MathTransform.hpp"
#include "../MathClasses/Matrix.hpp"

template <class MatrixType>
int Game::Lua::CLibraries::Matrix::TemplatedFunctions<MatrixType>::__new(
    lua_State* State) {
    typename MatrixType::ColumnType* Row1;
    MatrixType* NewMatrix;

    NewMatrix =
        static_cast<MatrixType*>(lua_newuserdata(State, sizeof(MatrixType)));

    lua_pushvalue(State, lua_upvalueindex(1));
    lua_setmetatable(State, -2);

    // if only 1 value was passed
    // check if it's 2 because we just made the new matrix
    if (lua_gettop(State) == 2) {
        *NewMatrix = MatrixType(
            LuaHelper::ToTypename<typename MatrixType::ComponentType>(State,
                                                                      1));
        return 1;
    }
    lua_insert(State, 1);

    if (Row1 = reinterpret_cast<typename MatrixType::ColumnType*>(
            LuaHelper::TestMetatable(State, 1, lua_upvalueindex(2)));
        Row1 == NULL) {
        lua_settop(State, (MatrixType::RowCount * MatrixType::ColumnCount) + 1);

        for (int i = 2; i <= MatrixType::RowCount * MatrixType::ColumnCount;
             ++i) {
            reinterpret_cast<typename MatrixType::ColumnType::ComponentType*>(
                NewMatrix)[i - 2] = luaL_optnumber(State, i, 0.0);
        }

    } else {
        lua_settop(State, MatrixType::ColumnCount + 1);

        NewMatrix->R0 = *Row1;
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

    *Result =
        static_cast<MatrixType*>(
            LuaHelper::CheckMetatable(State, 1, lua_upvalueindex(1)))
            ->Lerp(*reinterpret_cast<MatrixType*>(LuaHelper::CheckMetatable(
                       State, 2, lua_upvalueindex(1))),
                   static_cast<float>(luaL_checknumber(State, 3)));
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

int Game::Lua::CLibraries::Matrix::__RotateAxis(lua_State* State) {
    Math::Mat4* Result = reinterpret_cast<Math::Mat4*>(
        lua_newuserdata(State, sizeof(Math::Mat4)));

    luaL_getmetatable(State, "Mat4");
    lua_setmetatable(State, -2);

    *Result = Math::Transform::RotateAxis(
        *reinterpret_cast<Math::Mat4*>(luaL_checkudata(State, 1, "Mat4")),
        static_cast<float>(luaL_checknumber(State, 2)),
        *reinterpret_cast<Math::Vector3*>(
            luaL_checkudata(State, 3, "Vector3")));

    return 1;
}

int Game::Lua::CLibraries::Matrix::__Rotate(lua_State* State) {
    Math::Mat4* Result = reinterpret_cast<Math::Mat4*>(
        lua_newuserdata(State, sizeof(Math::Mat4)));

    luaL_getmetatable(State, "Mat4");
    lua_setmetatable(State, -2);

    *Result = Math::Transform::Rotate(
        *reinterpret_cast<Math::Mat4*>(luaL_checkudata(State, 1, "Mat4")),
        *reinterpret_cast<Math::Vector3*>(
            luaL_checkudata(State, 2, "Vector3")));

    return 1;
}

template int Game::Lua::CLibraries::Matrix::TemplatedFunctions<
    Math::Mat2>::__new(lua_State*);
template int Game::Lua::CLibraries::Matrix::TemplatedFunctions<
    Math::Mat3>::__new(lua_State*);
template int Game::Lua::CLibraries::Matrix::TemplatedFunctions<
    Math::Mat4>::__new(lua_State*);

template int Game::Lua::CLibraries::Matrix::TemplatedFunctions<
    Math::Mat2>::__tostring(lua_State*);
template int Game::Lua::CLibraries::Matrix::TemplatedFunctions<
    Math::Mat3>::__tostring(lua_State*);
template int Game::Lua::CLibraries::Matrix::TemplatedFunctions<
    Math::Mat4>::__tostring(lua_State*);

template int Game::Lua::CLibraries::Matrix::TemplatedFunctions<
    Math::Mat2>::__index(lua_State*);
template int Game::Lua::CLibraries::Matrix::TemplatedFunctions<
    Math::Mat3>::__index(lua_State*);
template int Game::Lua::CLibraries::Matrix::TemplatedFunctions<
    Math::Mat4>::__index(lua_State*);

template int Game::Lua::CLibraries::Matrix::TemplatedFunctions<
    Math::Mat2>::__newindex(lua_State*);
template int Game::Lua::CLibraries::Matrix::TemplatedFunctions<
    Math::Mat3>::__newindex(lua_State*);
template int Game::Lua::CLibraries::Matrix::TemplatedFunctions<
    Math::Mat4>::__newindex(lua_State*);

template int Game::Lua::CLibraries::Matrix::TemplatedFunctions<
    Math::Mat2>::__mul(lua_State*);
template int Game::Lua::CLibraries::Matrix::TemplatedFunctions<
    Math::Mat3>::__mul(lua_State*);
template int Game::Lua::CLibraries::Matrix::TemplatedFunctions<
    Math::Mat4>::__mul(lua_State*);

template int Game::Lua::CLibraries::Matrix::TemplatedFunctions<
    Math::Mat2>::__lerp(lua_State*);
template int Game::Lua::CLibraries::Matrix::TemplatedFunctions<
    Math::Mat3>::__lerp(lua_State*);
template int Game::Lua::CLibraries::Matrix::TemplatedFunctions<
    Math::Mat4>::__lerp(lua_State*);

template int Game::Lua::CLibraries::Matrix::TemplatedFunctions<
    Math::Mat2>::__eq(lua_State*);
template int Game::Lua::CLibraries::Matrix::TemplatedFunctions<
    Math::Mat3>::__eq(lua_State*);
template int Game::Lua::CLibraries::Matrix::TemplatedFunctions<
    Math::Mat4>::__eq(lua_State*);