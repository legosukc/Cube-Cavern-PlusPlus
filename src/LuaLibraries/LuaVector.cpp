#include "LuaVector.hpp"

#include "../../include/VM/lua.h"
#include "../../include/VM/lualib.h"

#include <cmath>
#include <string>
#include <type_traits>

#include "../FunctionHeaders/LuaHelper.hpp"

#include "../MathClasses/Vector2.hpp"
#include "../MathClasses/Vector3.hpp"
#include "../MathClasses/Vector4.hpp"

template <class VectorType>
int Game::Lua::CLibraries::Vector::__new(lua_State* State) {
    lua_settop(State, VectorType::ComponentCount);

    VectorType* const NewVector =
        static_cast<VectorType*>(lua_newuserdata(State, sizeof(VectorType)));
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
int Game::Lua::CLibraries::Vector::__tostring(lua_State* State) {
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
int Game::Lua::CLibraries::Vector::__index(lua_State* State) {
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
                                      'X' + static_cast<char>(
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
                                Index_Int > VectorType::ComponentCount - 1) {
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
int Game::Lua::CLibraries::Vector::__newindex(lua_State* State) {
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
                luaL_error(State, "Cannot set index of Vector: Invalid index: ",
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
                LuaHelper::ToTypename<typename VectorType::ComponentType>(State,
                                                                          3);
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
                LuaHelper::ToTypename<typename VectorType::ComponentType>(State,
                                                                          3);
            // static_cast<float*>(lua_touserdata(State, 1))[Index_Int] =
            // luaL_checknumber(State, 3);
            break;

        default:
            luaL_error(State,
                       "Cannot set component of Vector3: Invalid index type '",
                       lua_typename(State, IndexType), "'.");
    }

    return 0;
}

template <class VectorType>
int Game::Lua::CLibraries::Vector::__add(lua_State* State) {
    VectorType *Result, *B;

    lua_settop(State, 2);
    lua_pushvalue(State, lua_upvalueindex(1));

    Result =
        static_cast<VectorType*>(lua_newuserdata(State, sizeof(VectorType)));
    lua_pushvalue(State, 3);
    lua_setmetatable(State, 4);

    *Result = *static_cast<VectorType*>(LuaHelper::CheckMetatable(State, 1, 3));

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
int Game::Lua::CLibraries::Vector::__sub(lua_State* State) {
    VectorType *Result, *B;

    lua_settop(State, 2);
    lua_pushvalue(State, lua_upvalueindex(1));

    Result =
        static_cast<VectorType*>(lua_newuserdata(State, sizeof(VectorType)));
    lua_pushvalue(State, 3);
    lua_setmetatable(State, 4);

    *Result = *static_cast<VectorType*>(LuaHelper::CheckMetatable(State, 1, 3));

    if (B = static_cast<VectorType*>(LuaHelper::TestMetatable(State, 2, 3));
        B != NULL) {
        *Result -= *B;
    } else {
        *Result -=
            LuaHelper::ToTypename<typename VectorType::ComponentType>(State, 2);
    }

    return 1;
}

template <class VectorType>
int Game::Lua::CLibraries::Vector::__unm(lua_State* State) {
    lua_pushvalue(State, lua_upvalueindex(1));
    VectorType* Result =
        static_cast<VectorType*>(lua_newuserdata(State, sizeof(VectorType)));
    lua_pushvalue(State, -2);
    lua_setmetatable(State, -2);

    *Result = static_cast<VectorType*>(LuaHelper::CheckMetatable(State, 1, -2))
                  ->operator-();
    return 1;
}

template <class VectorType>
int Game::Lua::CLibraries::Vector::__mul(lua_State* State) {
    VectorType *Result, *B;

    lua_settop(State, 2);
    lua_pushvalue(State, lua_upvalueindex(1));

    Result =
        static_cast<VectorType*>(lua_newuserdata(State, sizeof(VectorType)));
    lua_pushvalue(State, 3);
    lua_setmetatable(State, 4);

    *Result = *static_cast<VectorType*>(LuaHelper::CheckMetatable(State, 1, 3));

    if (B = static_cast<VectorType*>(LuaHelper::TestMetatable(State, 2, 3));
        B != NULL) {
        *Result *= *B;
    } else {
        *Result *=
            LuaHelper::ToTypename<typename VectorType::ComponentType>(State, 2);
    }

    return 1;
}

template <class VectorType>
int Game::Lua::CLibraries::Vector::__div(lua_State* State) {
    VectorType *Result, *B;

    lua_settop(State, 2);
    lua_pushvalue(State, lua_upvalueindex(1));

    Result =
        static_cast<VectorType*>(lua_newuserdata(State, sizeof(VectorType)));
    lua_pushvalue(State, 3);
    lua_setmetatable(State, 4);

    *Result = *static_cast<VectorType*>(LuaHelper::CheckMetatable(State, 1, 3));

    if (B = static_cast<VectorType*>(LuaHelper::TestMetatable(State, 2, 3));
        B != NULL) {
        *Result /= *B;
    } else {
        *Result /=
            LuaHelper::ToTypename<typename VectorType::ComponentType>(State, 2);
    }

    return 1;
}

template <class VectorType>
int Game::Lua::CLibraries::Vector::__idiv(lua_State* State) {
    VectorType *Result, *B;

    lua_settop(State, 2);
    lua_pushvalue(State, lua_upvalueindex(1));

    Result =
        static_cast<VectorType*>(lua_newuserdata(State, sizeof(VectorType)));
    lua_pushvalue(State, 3);
    lua_setmetatable(State, 4);

    *Result = *static_cast<VectorType*>(LuaHelper::CheckMetatable(State, 1, 3));

    if (B = static_cast<VectorType*>(LuaHelper::TestMetatable(State, 2, 3));
        B != NULL) {
        *Result /= *B;
    } else {
        *Result /=
            LuaHelper::ToTypename<typename VectorType::ComponentType>(State, 2);
    }

    return 1;
}

template <class VectorType>
int Game::Lua::CLibraries::Vector::__mod(lua_State* State) {
    VectorType *Result, *B;

    lua_settop(State, 2);
    lua_pushvalue(State, lua_upvalueindex(1));

    Result =
        static_cast<VectorType*>(lua_newuserdata(State, sizeof(VectorType)));
    lua_pushvalue(State, 3);
    lua_setmetatable(State, 4);

    *Result = *static_cast<VectorType*>(LuaHelper::CheckMetatable(State, 1, 3));

    if (B = static_cast<VectorType*>(LuaHelper::TestMetatable(State, 2, 3));
        B != NULL) {
        *Result %= *B;
    } else {
        *Result %=
            LuaHelper::ToTypename<typename VectorType::ComponentType>(State, 2);
    }

    return 1;
}

template <class VectorType>
int Game::Lua::CLibraries::Vector::__eq(lua_State* State) {
    lua_settop(State, 2);
    lua_pushvalue(State, lua_upvalueindex(1));

    lua_pushboolean(
        State,
        *static_cast<VectorType*>(LuaHelper::CheckMetatable(State, 1, 3)) ==
            *static_cast<VectorType*>(LuaHelper::CheckMetatable(State, 2, 3)));
    return 1;
}

template <class VectorType>
int Game::Lua::CLibraries::Vector::__Magnitude(lua_State* State) {
    LuaHelper::Push<typename VectorType::ComponentType>(
        State, static_cast<VectorType*>(
                   LuaHelper::CheckMetatable(State, 1, lua_upvalueindex(1)))
                   ->Magnitude());
    return 1;
}

template <class VectorType>
int Game::Lua::CLibraries::Vector::__Dot(lua_State* State) {
    LuaHelper::Push<typename VectorType::ComponentType>(
        State, static_cast<VectorType*>(
                   LuaHelper::CheckMetatable(State, 1, lua_upvalueindex(1)))
                   ->Dot(*static_cast<VectorType*>(LuaHelper::CheckMetatable(
                       State, 2, lua_upvalueindex(1)))));
    return 1;
}

template <class VectorType>
int Game::Lua::CLibraries::Vector::__Cross(lua_State* State) {
    static_assert(
        std::is_base_of_v<Math::etc::_base_vector3<
                              typename VectorType::ComponentType, VectorType>,
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
int Game::Lua::CLibraries::Vector::__Normalize(lua_State* State) {
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
int Game::Lua::CLibraries::Vector::__Lerp(lua_State* State) {
    lua_settop(State, 3);
    lua_pushvalue(State, lua_upvalueindex(1));

    const VectorType* A =
        static_cast<VectorType*>(LuaHelper::CheckMetatable(State, 1, 4));
    const VectorType* B =
        static_cast<VectorType*>(LuaHelper::CheckMetatable(State, 2, 4));

    VectorType* Result =
        static_cast<VectorType*>(lua_newuserdata(State, sizeof(VectorType)));

    if (lua_isnumber(State, 3)) {
        *Result = A->Lerp(*B, static_cast<float>(lua_tonumber(State, 3)));
    } else if (const VectorType* AlphaVector = static_cast<VectorType*>(
                   LuaHelper::CheckMetatable(State, 3, 4));
               AlphaVector != NULL) {
        *Result = A->Lerp(*B, *AlphaVector);
    } else {
        // TODO: replace 'Vector' with correct type name.
        luaL_typeerror(State, 3, "Vector or number");
        return 0;
    }

    lua_pushvalue(State, 4);
    lua_setmetatable(State, 5);
    return 1;
}

template <class VectorType>
int Game::Lua::CLibraries::Vector::__StripNaN(lua_State* State) {
    lua_settop(State, 1);
    lua_pushvalue(State, lua_upvalueindex(1));

    const VectorType BaseVector =
        *static_cast<VectorType*>(LuaHelper::CheckMetatable(State, 1, 2));

    VectorType* NewVector =
        static_cast<VectorType*>(lua_newuserdata(State, sizeof(VectorType)));

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
int Game::Lua::CLibraries::Vector::__StripY(lua_State* State) {
    lua_settop(State, 1);

    VectorType* NewVector =
        static_cast<VectorType*>(lua_newuserdata(State, sizeof(VectorType)));

    lua_pushvalue(State, lua_upvalueindex(1));

    *NewVector =
        *static_cast<VectorType*>(LuaHelper::CheckMetatable(State, 1, 3));
    NewVector->Y = static_cast<typename VectorType::ComponentType>(0);

    lua_setmetatable(State, 2);
    return 1;
}


// C++ is a SHIT language - see below

/////////////
/* VECTOR2 */
/////////////

template int Game::Lua::CLibraries::Vector::__new<Math::Vector2>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__tostring<Math::Vector2>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__index<Math::Vector2>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__newindex<Math::Vector2>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__add<Math::Vector2>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__sub<Math::Vector2>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__unm<Math::Vector2>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__mul<Math::Vector2>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__div<Math::Vector2>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__idiv<Math::Vector2>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__mod<Math::Vector2>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__eq<Math::Vector2>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__Magnitude<Math::Vector2>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__Dot<Math::Vector2>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__Normalize<Math::Vector2>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__Lerp<Math::Vector2>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__StripNaN<Math::Vector2>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__StripY<Math::Vector2>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__new<Math::IVector2>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__tostring<Math::IVector2>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__index<Math::IVector2>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__newindex<Math::IVector2>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__add<Math::IVector2>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__sub<Math::IVector2>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__unm<Math::IVector2>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__mul<Math::IVector2>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__div<Math::IVector2>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__idiv<Math::IVector2>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__mod<Math::IVector2>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__eq<Math::IVector2>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__Magnitude<Math::IVector2>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__Dot<Math::IVector2>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__StripNaN<Math::IVector2>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__StripY<Math::IVector2>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__new<Math::UVector2>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__tostring<Math::UVector2>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__index<Math::UVector2>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__newindex<Math::UVector2>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__add<Math::UVector2>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__sub<Math::UVector2>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__unm<Math::UVector2>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__mul<Math::UVector2>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__div<Math::UVector2>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__idiv<Math::UVector2>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__mod<Math::UVector2>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__eq<Math::UVector2>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__Magnitude<Math::UVector2>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__Dot<Math::UVector2>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__StripNaN<Math::UVector2>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__StripY<Math::UVector2>(
    lua_State* State);

/////////////
/* VECTOR3 */
/////////////

template int Game::Lua::CLibraries::Vector::__new<Math::Vector3>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__tostring<Math::Vector3>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__index<Math::Vector3>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__newindex<Math::Vector3>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__add<Math::Vector3>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__sub<Math::Vector3>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__unm<Math::Vector3>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__mul<Math::Vector3>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__div<Math::Vector3>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__idiv<Math::Vector3>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__mod<Math::Vector3>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__eq<Math::Vector3>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__Magnitude<Math::Vector3>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__Dot<Math::Vector3>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__Normalize<Math::Vector3>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__Lerp<Math::Vector3>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__StripNaN<Math::Vector3>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__StripY<Math::Vector3>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__Cross<Math::Vector3>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__new<Math::IVector3>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__tostring<Math::IVector3>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__index<Math::IVector3>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__newindex<Math::IVector3>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__add<Math::IVector3>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__sub<Math::IVector3>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__unm<Math::IVector3>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__mul<Math::IVector3>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__div<Math::IVector3>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__idiv<Math::IVector3>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__mod<Math::IVector3>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__eq<Math::IVector3>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__Magnitude<Math::IVector3>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__Dot<Math::IVector3>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__StripNaN<Math::IVector3>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__StripY<Math::IVector3>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__Cross<Math::IVector3>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__new<Math::UVector3>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__tostring<Math::UVector3>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__index<Math::UVector3>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__newindex<Math::UVector3>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__add<Math::UVector3>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__sub<Math::UVector3>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__unm<Math::UVector3>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__mul<Math::UVector3>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__div<Math::UVector3>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__idiv<Math::UVector3>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__mod<Math::UVector3>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__eq<Math::UVector3>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__Magnitude<Math::UVector3>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__Dot<Math::UVector3>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__StripNaN<Math::UVector3>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__StripY<Math::UVector3>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__Cross<Math::UVector3>(
    lua_State* State);

/////////////
/* VECTOR4 */
/////////////

template int Game::Lua::CLibraries::Vector::__new<Math::Vector4>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__tostring<Math::Vector4>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__index<Math::Vector4>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__newindex<Math::Vector4>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__add<Math::Vector4>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__sub<Math::Vector4>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__unm<Math::Vector4>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__mul<Math::Vector4>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__div<Math::Vector4>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__idiv<Math::Vector4>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__mod<Math::Vector4>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__eq<Math::Vector4>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__Magnitude<Math::Vector4>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__Dot<Math::Vector4>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__Normalize<Math::Vector4>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__Lerp<Math::Vector4>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__StripNaN<Math::Vector4>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__StripY<Math::Vector4>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__new<Math::IVector4>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__tostring<Math::IVector4>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__index<Math::IVector4>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__newindex<Math::IVector4>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__add<Math::IVector4>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__sub<Math::IVector4>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__unm<Math::IVector4>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__mul<Math::IVector4>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__div<Math::IVector4>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__idiv<Math::IVector4>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__mod<Math::IVector4>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__eq<Math::IVector4>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__Magnitude<Math::IVector4>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__Dot<Math::IVector4>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__StripNaN<Math::IVector4>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__StripY<Math::IVector4>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__new<Math::UVector4>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__tostring<Math::UVector4>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__index<Math::UVector4>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__newindex<Math::UVector4>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__add<Math::UVector4>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__sub<Math::UVector4>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__unm<Math::UVector4>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__mul<Math::UVector4>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__div<Math::UVector4>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__idiv<Math::UVector4>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__mod<Math::UVector4>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__eq<Math::UVector4>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__Magnitude<Math::UVector4>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__Dot<Math::UVector4>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__StripNaN<Math::UVector4>(
    lua_State* State);

template int Game::Lua::CLibraries::Vector::__StripY<Math::UVector4>(
    lua_State* State);