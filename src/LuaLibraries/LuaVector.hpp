#pragma once

#include "../../include/VM/lua.h"
#include "../../include/VM/lualib.h"

#include <cmath>
#include <string>
#include <type_traits>

#include "../FunctionHeaders/LuaHelper.hpp"

#include "../MathClasses/Vector2.hpp"
#include "../MathClasses/Vector3.hpp"
#include "../MathClasses/Vector4.hpp"

namespace Game::Lua::CLibraries::Vector {

    template <class VectorType>
    static int __new(lua_State* State) {
        lua_settop(State, VectorType::ComponentCount);

        VectorType* const NewVector = static_cast<VectorType*>(
            lua_newuserdata(State, sizeof(VectorType)));
        lua_pushvalue(State, lua_upvalueindex(1));
        lua_setmetatable(State, VectorType::ComponentCount + 1);

        for (int i = 1; i <= VectorType::ComponentCount; ++i) {
            if CONSTEXPR_IF (std::is_integral_v<
                                 typename VectorType::ComponentType>) {
                (*NewVector)[i - 1] =
                    static_cast<typename VectorType::ComponentType>(
                        luaL_optinteger(State, i, 0));
            } else {
                (*NewVector)[i - 1] =
                    static_cast<typename VectorType::ComponentType>(
                        luaL_optnumber(State, i, 0.0));
            }
        }

        return 1;
    }

    template <class VectorType>
    static int __tostring(lua_State* State) {
        std::string ComponentString;
        int i;
        std::string VectorString;
        const VectorType* Vector;

        Vector = static_cast<VectorType*>(
            LuaHelper::CheckMetatable(State, 1, lua_upvalueindex(1)));

        VectorString = std::to_string(Vector->X);
        for (i = 1; i < VectorType::ComponentCount; ++i) {
            ComponentString = std::to_string(Vector->operator[](i));

            VectorString.reserve(ComponentString.size() + 2);
            VectorString.append(", ");
            VectorString.append(ComponentString);
        }

        lua_pushstring(State, VectorString.c_str());
        return 1;
    }

    template <class VectorType>
    static int __index(lua_State* State) {
        lua_Integer Index_Int;
        const char* Index_Str;
        size_t IndexLength_Str;
        int IndexType;
        VectorType* VectorUD;

        lua_settop(State, 2);

        VectorUD = static_cast<VectorType*>(
            LuaHelper::CheckMetatable(State, 1, lua_upvalueindex(1)));

        IndexType = lua_type(State, 2);
        switch (IndexType) {
            case LUA_TSTRING:

                Index_Str = lua_tolstring(State, 2, &IndexLength_Str);
                if (IndexLength_Str == 1) {
                    likely_branch if (*Index_Str >= 'X' &&
                                      *Index_Str <
                                          'X' +
                                              static_cast<char>(
                                                  VectorType::ComponentCount)) {
                        LuaHelper::Push<typename VectorType::ComponentType>(
                            State, (*VectorUD)[*Index_Str - 'X']);
                        break;
                    }
                }

                lua_pushvalue(State, 2);
                lua_gettable(State, lua_upvalueindex(1));
                break;

            case LUA_TNUMBER:

                Index_Int = lua_tointeger(State, 2);

                unlikely_branch if (Index_Int < 0 ||
                                    Index_Int >
                                        VectorType::ComponentCount - 1) {
                    luaL_error(
                        State,
                        "Cannot index Vector: Numerical index out of range: (",
                        Index_Int, ").");
                }

                LuaHelper::Push<typename VectorType::ComponentType>(
                    State, (*VectorUD)[Index_Int]);
                break;

            default:
                luaL_error(State, "Cannot index Vector with type '",
                           lua_typename(State, IndexType),
                           "'. Expected 'string' or 'integer'.");
        }

        return 1;
    }

    template <class VectorType>
    static int __newindex(lua_State* State) {
        lua_Integer Index_Int;
        const char* Index_Str;
        size_t IndexLength_Str;
        int IndexType;
        VectorType* VectorUD;

        lua_settop(State, 3);

        VectorUD = static_cast<VectorType*>(
            LuaHelper::CheckMetatable(State, 1, lua_upvalueindex(1)));

        IndexType = lua_type(State, 2);
        switch (IndexType) {
            case LUA_TSTRING:

                Index_Str = lua_tolstring(State, 2, &IndexLength_Str);

                unlikely_branch if (IndexLength_Str > 1) {
                    luaL_error(State,
                               "Cannot set index of Vector: Invalid index: ",
                               Index_Str);
                }

                unlikely_branch if (Index_Str[0] < 'X' ||
                                    Index_Str[0] >
                                        'X' + VectorType::ComponentCount - 1) {
                    luaL_error(State,
                               "Cannot set component of Vector: Invalid "
                               "component index: ",
                               Index_Str);
                }

                (*VectorUD)[Index_Str[0] - 'X'] =
                    LuaHelper::ToTypename<typename VectorType::ComponentType>(
                        State, 3);
                // static_cast<float*>(lua_touserdata(State, 1))[Index_Str[0] -
                // 'X'] = luaL_checknumber(State, 3);
                break;

            case LUA_TNUMBER:

                Index_Int = lua_tointeger(State, 2);

                unlikely_branch if (Index_Int < 0 || Index_Int > 2) {
                    luaL_error(State,
                               "Cannot set component of Vector3: Numerical "
                               "index out of range (",
                               lua_tostring(State, 2), ").");
                }

                (*VectorUD)[Index_Int] =
                    LuaHelper::ToTypename<typename VectorType::ComponentType>(
                        State, 3);
                // static_cast<float*>(lua_touserdata(State, 1))[Index_Int] =
                // luaL_checknumber(State, 3);
                break;

            default:
                luaL_error(
                    State,
                    "Cannot set component of Vector3: Invalid index type '",
                    lua_typename(State, IndexType), "'.");
        }

        return 0;
    }

    template <class VectorType>
    static int __add(lua_State* State) {
        VectorType *Result, *B;

        lua_settop(State, 2);
        lua_pushvalue(State, lua_upvalueindex(1));

        Result = static_cast<VectorType*>(
            lua_newuserdata(State, sizeof(VectorType)));
        lua_pushvalue(State, 3);
        lua_setmetatable(State, 4);

        *Result =
            *static_cast<VectorType*>(LuaHelper::CheckMetatable(State, 1, 3));

        if (B = static_cast<VectorType*>(LuaHelper::TestMetatable(State, 2, 3));
            B != NULL) {
            *Result += *B;
        } else {
            Result->operator+=(
                LuaHelper::ToTypename<typename VectorType::ComponentType>(State,
                                                                          2));
        }

        return 1;
    }

    template <class VectorType>
    static int __sub(lua_State* State) {
        VectorType *Result, *B;

        lua_settop(State, 2);
        lua_pushvalue(State, lua_upvalueindex(1));

        Result = static_cast<VectorType*>(
            lua_newuserdata(State, sizeof(VectorType)));
        lua_pushvalue(State, 3);
        lua_setmetatable(State, 4);

        *Result =
            *static_cast<VectorType*>(LuaHelper::CheckMetatable(State, 1, 3));

        if (B = static_cast<VectorType*>(LuaHelper::TestMetatable(State, 2, 3));
            B != NULL) {
            *Result -= *B;
        } else {
            *Result -=
                LuaHelper::ToTypename<typename VectorType::ComponentType>(State,
                                                                          2);
        }

        return 1;
    }

    template <class VectorType>
    static int __unm(lua_State* State) {
        lua_pushvalue(State, lua_upvalueindex(1));
        VectorType* Result = static_cast<VectorType*>(
            lua_newuserdata(State, sizeof(VectorType)));
        lua_pushvalue(State, -2);
        lua_setmetatable(State, -2);

        *Result =
            static_cast<VectorType*>(LuaHelper::CheckMetatable(State, 1, -2))
                ->operator-();
        return 1;
    }

    template <class VectorType>
    static int __mul(lua_State* State) {
        VectorType *Result, *B;

        lua_settop(State, 2);
        lua_pushvalue(State, lua_upvalueindex(1));

        Result = static_cast<VectorType*>(
            lua_newuserdata(State, sizeof(VectorType)));
        lua_pushvalue(State, 3);
        lua_setmetatable(State, 4);

        *Result =
            *static_cast<VectorType*>(LuaHelper::CheckMetatable(State, 1, 3));

        if (B = static_cast<VectorType*>(LuaHelper::TestMetatable(State, 2, 3));
            B != NULL) {
            *Result *= *B;
        } else {
            *Result *=
                LuaHelper::ToTypename<typename VectorType::ComponentType>(State,
                                                                          2);
        }

        return 1;
    }

    template <class VectorType>
    static int __div(lua_State* State) {
        VectorType *Result, *B;

        lua_settop(State, 2);
        lua_pushvalue(State, lua_upvalueindex(1));

        Result = static_cast<VectorType*>(
            lua_newuserdata(State, sizeof(VectorType)));
        lua_pushvalue(State, 3);
        lua_setmetatable(State, 4);

        *Result =
            *static_cast<VectorType*>(LuaHelper::CheckMetatable(State, 1, 3));

        if (B = static_cast<VectorType*>(LuaHelper::TestMetatable(State, 2, 3));
            B != NULL) {
            *Result /= *B;
        } else {
            *Result /=
                LuaHelper::ToTypename<typename VectorType::ComponentType>(State,
                                                                          2);
        }

        return 1;
    }

    template <class VectorType>
    static int __idiv(lua_State* State) {
        VectorType *Result, *B;

        lua_settop(State, 2);
        lua_pushvalue(State, lua_upvalueindex(1));

        Result = static_cast<VectorType*>(
            lua_newuserdata(State, sizeof(VectorType)));
        lua_pushvalue(State, 3);
        lua_setmetatable(State, 4);

        *Result =
            *static_cast<VectorType*>(LuaHelper::CheckMetatable(State, 1, 3));

        if (B = static_cast<VectorType*>(LuaHelper::TestMetatable(State, 2, 3));
            B != NULL) {
            *Result /= *B;
        } else {
            *Result /=
                LuaHelper::ToTypename<typename VectorType::ComponentType>(State,
                                                                          2);
        }

        return 1;
    }

    template <class VectorType>
    static int __mod(lua_State* State) {
        VectorType *Result, *B;

        lua_settop(State, 2);
        lua_pushvalue(State, lua_upvalueindex(1));

        Result = static_cast<VectorType*>(
            lua_newuserdata(State, sizeof(VectorType)));
        lua_pushvalue(State, 3);
        lua_setmetatable(State, 4);

        *Result =
            *static_cast<VectorType*>(LuaHelper::CheckMetatable(State, 1, 3));

        if (B = static_cast<VectorType*>(LuaHelper::TestMetatable(State, 2, 3));
            B != NULL) {
            *Result %= *B;
        } else {
            *Result %=
                LuaHelper::ToTypename<typename VectorType::ComponentType>(State,
                                                                          2);
        }

        return 1;
    }

    template <class VectorType>
    static int __eq(lua_State* State) {
        lua_settop(State, 2);
        lua_pushvalue(State, lua_upvalueindex(1));

        lua_pushboolean(State, *static_cast<VectorType*>(
                                   LuaHelper::CheckMetatable(State, 1, 3)) ==
                                   *static_cast<VectorType*>(
                                       LuaHelper::CheckMetatable(State, 2, 3)));
        return 1;
    }

    template <class VectorType>
    static int __Magnitude(lua_State* State) {
        LuaHelper::Push<typename VectorType::ComponentType>(
            State, static_cast<VectorType*>(
                       LuaHelper::CheckMetatable(State, 1, lua_upvalueindex(1)))
                       ->Magnitude());
        return 1;
    }

    template <class VectorType>
    static int __Dot(lua_State* State) {
        LuaHelper::Push<typename VectorType::ComponentType>(
            State,
            static_cast<VectorType*>(
                LuaHelper::CheckMetatable(State, 1, lua_upvalueindex(1)))
                ->Dot(*static_cast<VectorType*>(
                    LuaHelper::CheckMetatable(State, 2, lua_upvalueindex(1)))));
        return 1;
    }

    template <class VectorType>
    static int __Cross(lua_State* State) {
        static_assert(
            std::is_base_of_v<
                Math::etc::_base_vector3<typename VectorType::ComponentType,
                                         VectorType>,
                VectorType>,
            "fuck.");

        lua_settop(State, 2);
        lua_pushvalue(State, lua_upvalueindex(1));

        *static_cast<VectorType*>(lua_newuserdata(State, sizeof(VectorType))) =
            static_cast<VectorType*>(LuaHelper::CheckMetatable(State, 1, 3))
                ->Cross(*static_cast<VectorType*>(
                    LuaHelper::CheckMetatable(State, 2, 3)));

        lua_pushvalue(State, 3);
        lua_setmetatable(State, 4);
        return 1;
    }

    template <class VectorType>
    static int __Normalize(lua_State* State) {
        lua_settop(State, 1);
        lua_pushvalue(State, lua_upvalueindex(1));

        *static_cast<VectorType*>(lua_newuserdata(State, sizeof(VectorType))) =
            static_cast<VectorType*>(LuaHelper::CheckMetatable(State, 1, 2))
                ->Normalize();

        lua_pushvalue(State, 2);
        lua_setmetatable(State, 3);
        return 1;
    }

    template <class VectorType>
    static int __Lerp(lua_State* State) {
        lua_settop(State, 1);
        lua_pushvalue(State, lua_upvalueindex(1));

        const VectorType* A =
            static_cast<VectorType*>(LuaHelper::CheckMetatable(State, 1, 2));
        const VectorType* B =
            static_cast<VectorType*>(LuaHelper::CheckMetatable(State, 2, 2));

        VectorType* Result = static_cast<VectorType*>(
            lua_newuserdata(State, sizeof(VectorType)));

        if (lua_isnumber(State, 3)) {
            *Result = A->Lerp(*B, static_cast<float>(lua_tonumber(State, 3)));
        } else if (const VectorType* AlphaVector = static_cast<VectorType*>(
                       LuaHelper::CheckMetatable(State, 3, 2));
                   AlphaVector != NULL) {
                    *Result = A->Lerp(*B, *AlphaVector);
        } else {
            // TODO: replace 'Vector' with correct type name.
            luaL_typeerror(State, 3, "Vector or number");
            return -1;
        }

        lua_pushvalue(State, 2);
        lua_setmetatable(State, 3);
        return 1;
    }

    template <class VectorType>
    static int __StripNaN(lua_State* State) {
        lua_settop(State, 1);
        lua_pushvalue(State, lua_upvalueindex(1));

        const VectorType BaseVector =
            *static_cast<VectorType*>(LuaHelper::CheckMetatable(State, 1, 2));

        VectorType* NewVector = static_cast<VectorType*>(
            lua_newuserdata(State, sizeof(VectorType)));

        for (Uint8 i = 0; i < VectorType::ComponentCount; ++i) {
            (*NewVector)[i] =
                std::isnan(BaseVector[i])
                    ? static_cast<typename VectorType::ComponentType>(0)
                    : BaseVector[i];
        }

        lua_pushvalue(State, 2);
        lua_setmetatable(State, 3);
        return 1;
    }

    // Returns a new vector with the Y component set to 0.
    template <class VectorType>
    static int __StripY(lua_State* State) {
        lua_settop(State, 1);

        VectorType* NewVector = static_cast<VectorType*>(
            lua_newuserdata(State, sizeof(VectorType)));

        lua_pushvalue(State, lua_upvalueindex(1));

        *NewVector =
            *static_cast<VectorType*>(LuaHelper::CheckMetatable(State, 1, 3));
        NewVector->Y = static_cast<typename VectorType::ComponentType>(0);

        lua_setmetatable(State, 2);
        return 1;
    }

    template <class VectorType>
    static inline void SetupVector(lua_State* State, const char* VectorName) {
        LuaHelper::StackTableReference VectorTable, VectorMetatable;

        VectorTable = LuaHelper::StackTableReference(State, 0, 1);
        VectorMetatable = LuaHelper::StackTableReference(State, VectorName);

        // for (Uint8 i = 0; i < 14 + std::is_floating_point_v<typename
        // VectorType::ComponentType>; ++i) {
        // VectorMetatable.PushReference(State);
        //}
        VectorMetatable.PushReference(State);
        VectorMetatable.SetKeyClosure(State, __tostring<VectorType>,
                                      "__tostring", 1);

        VectorMetatable.PushReference(State);
        VectorMetatable.SetKeyClosure(State, __add<VectorType>, "__add", 1);

        VectorMetatable.PushReference(State);
        VectorMetatable.SetKeyClosure(State, __sub<VectorType>, "__sub", 1);

        VectorMetatable.PushReference(State);
        VectorMetatable.SetKeyClosure(State, __unm<VectorType>, "__unm", 1);

        VectorMetatable.PushReference(State);
        VectorMetatable.SetKeyClosure(State, __mul<VectorType>, "__mul", 1);

        VectorMetatable.PushReference(State);
        VectorMetatable.SetKeyClosure(State, __div<VectorType>, "__div", 1);

        VectorMetatable.PushReference(State);
        VectorMetatable.SetKeyClosure(State, __idiv<VectorType>, "__idiv", 1);

        VectorMetatable.PushReference(State);
        VectorMetatable.SetKeyClosure(State, __mod<VectorType>, "__mod", 1);

        VectorMetatable.PushReference(State);
        VectorMetatable.SetKeyClosure(State, __eq<VectorType>, "__eq", 1);

        VectorMetatable.PushReference(State);
        VectorMetatable.SetKeyClosure(State, __Magnitude<VectorType>,
                                      "Magnitude", 1);

        VectorMetatable.PushReference(State);
        VectorMetatable.SetKeyClosure(State, __Dot<VectorType>, "Dot", 1);

        if CONSTEXPR_IF (std::is_base_of_v<
                             Math::etc::_base_vector3<
                                 typename VectorType::ComponentType,
                                 VectorType>,
                             VectorType>) {
            VectorMetatable.PushReference(State);
            VectorMetatable.SetKeyClosure(State, __Cross<VectorType>, "Cross",
                                          1);
        }

        if CONSTEXPR_IF (std::is_floating_point_v<
                             typename VectorType::ComponentType>) {
            VectorMetatable.PushReference(State);
            VectorMetatable.SetKeyClosure(State, __Normalize<VectorType>,
                                          "Normalize", 1);

            VectorMetatable.PushReference(State);
            VectorMetatable.SetKeyClosure(State, __StripNaN<VectorType>,
                                          "StripNaN", 1);
        }

        VectorMetatable.PushReference(State);
        VectorMetatable.SetKeyClosure(State, __StripY<VectorType>, "StripY", 1);

        VectorMetatable.PushReference(State);
        VectorMetatable.SetKeyClosure(State, __Lerp<VectorType>, "Lerp", 1);

        VectorMetatable.PushReference(State);
        VectorMetatable.SetKeyClosure(State, __index<VectorType>, "__index", 1);

        VectorMetatable.PushReference(State);
        VectorMetatable.SetKeyClosure(State, __newindex<VectorType>,
                                      "__newindex", 1);

        VectorTable.SetKeyClosure(State, __new<VectorType>, "new", 1);

        lua_setglobal(State, VectorName);
    }

    inline void Init(lua_State* State) {
        SetupVector<Math::Vector2>(State, "Vector2");
        SetupVector<Math::IVector2>(State, "IVector2");
        SetupVector<Math::UVector2>(State, "UVector2");

        SetupVector<Math::Vector3>(State, "Vector3");
        SetupVector<Math::IVector3>(State, "IVector3");
        SetupVector<Math::UVector3>(State, "UVector3");

        SetupVector<Math::Vector4>(State, "Vector4");
        SetupVector<Math::IVector4>(State, "IVector4");
        SetupVector<Math::UVector4>(State, "UVector4");
    }
}