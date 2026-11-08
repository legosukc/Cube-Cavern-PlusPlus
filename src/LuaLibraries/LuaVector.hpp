#pragma once

#include "../../include/VM/lua.h"

#include <type_traits>

#include "../FunctionHeaders/LuaHelper.hpp"

#include "../MathClasses/Vector2.hpp"
#include "../MathClasses/Vector3.hpp"
#include "../MathClasses/Vector4.hpp"

//hi
//a
namespace Game::Lua::CLibraries::Vector {

    template <class VectorType>
    int __new(lua_State* State);

    template <class VectorType>
    int __tostring(lua_State* State);

    template <class VectorType>
    int __index(lua_State* State);

    template <class VectorType>
    int __newindex(lua_State* State);

    template <class VectorType>
    int __add(lua_State* State);

    template <class VectorType>
    int __sub(lua_State* State);

    template <class VectorType>
    int __unm(lua_State* State);

    template <class VectorType>
    int __mul(lua_State* State);

    template <class VectorType>
    int __div(lua_State* State);

    template <class VectorType>
    int __idiv(lua_State* State);

    template <class VectorType>
    int __mod(lua_State* State);

    template <class VectorType>
    int __eq(lua_State* State);

    template <class VectorType>
    int __Magnitude(lua_State* State);

    template <class VectorType>
    int __Dot(lua_State* State);

    template <class VectorType>
    int __Cross(lua_State* State);

    template <class VectorType>
    int __Normalize(lua_State* State);

    template <class VectorType>
    int __Lerp(lua_State* State);

    template <class VectorType>
    int __StripNaN(lua_State* State);

    // Returns a new vector with the Y component set to 0.
    template <class VectorType>
    int __StripY(lua_State* State);

    template <class VectorType>
    inline void SetupVector(lua_State* State, const char* VectorName) {
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

            VectorMetatable.PushReference(State);
            VectorMetatable.SetKeyClosure(State, __Lerp<VectorType>, "Lerp", 1);
        }

        VectorMetatable.PushReference(State);
        VectorMetatable.SetKeyClosure(State, __StripY<VectorType>, "StripY", 1);

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