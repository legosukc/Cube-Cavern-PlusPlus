#pragma once

#include <lua-5.4.2/lua.hpp>

#include <type_traits>

#include "../Vector2.hpp"
#include "../Vector3.hpp"
#include "../Vector4.hpp"

namespace {

	template<class VectorType>
	static int __new(lua_State* State) {

		lua_settop(State, VectorType::ComponentCount);

		VectorType* const NewVector = static_cast<VectorType*>(lua_newuserdata(State, sizeof(VectorType)));
		lua_pushvalue(State, lua_upvalueindex(1));
		lua_setmetatable(State, VectorType::ComponentCount + 1);

		for (int i = 1; i <= VectorType::ComponentCount; ++i) {

			if CONSTEXPR_IF(std::is_integral_v<typename VectorType::ComponentType>) {
				(*NewVector)[i] = luaL_optinteger(State, i, 0);
			} else {
				(*NewVector)[i] = luaL_optnumber(State, i, 0.0);
			}
		}

		lua_rotate(State, 0, 1);
		return 1;
	}


	template<class VectorType>
	static int __tostring(lua_State* State) {

		lua_settop(State, 1);
		const VectorType* Vector = static_cast<VectorType*>(lua_touserdata(State, 1));

		LuaHelper::Push<typename VectorType::ComponentType>(State, (*Vector)[VectorType::ComponentCount - 1]);
		for (int i = VectorType::ComponentCount - 1; i > 0; --i) {

			std::cout << i << std::endl;
			lua_pushliteral(State, ", ");
			LuaHelper::Push<typename VectorType::ComponentType>(State, (*Vector)[i]);
		}
		lua_concat(State, VectorType::ComponentCount);

		lua_rotate(State, 0, 1);
		//lua_settop(State, 0);
		//lua_pushliteral(State, "hi");
		return 1;
	}

	template<class VectorType>
	static int __index(lua_State* State) {

		lua_Integer Index_Int;
		const char* Index_Str;
		size_t IndexLength_Str;
		int IndexType;

		IndexType = lua_type(State, 2);
		switch (IndexType) {
		case LUA_TSTRING:

			Index_Str = lua_tolstring(State, 2, &IndexLength_Str);
			if (IndexLength_Str == 1) {

				unlikely_branch
					if (Index_Str[0] >= 'X' && Index_Str[0] <= 'X' + VectorType::ComponentCount - 1) {
						luaL_error(State, "Cannot index Vector: Invalid component index: ", Index_Str);
					}

				LuaHelper::Push<typename VectorType::ComponentType>(State, (*static_cast<VectorType*>(lua_touserdata(State, 1)))[Index_Str[0] - 'X']);

			} else {
				lua_getmetatable(State, 1);
				lua_pushvalue(State, 2);
				lua_gettable(State, -2);
			}
			break;

		case LUA_TNUMBER:

			Index_Int = lua_tointeger(State, 2);

			unlikely_branch
				if (Index_Int < 0 || Index_Int > VectorType::ComponentCount - 1) {
					luaL_error(State, "Cannot index Vector: Numerical index out of range: (", Index_Int, ").");
				}

			LuaHelper::Push<typename VectorType::ComponentType>(State, (*static_cast<VectorType*>(lua_touserdata(State, 1)))[Index_Int]);
			break;

		default:
			luaL_error(State, "Cannot index Vector with type '", lua_typename(State, IndexType), "'. Expected 'string' or 'integer'.");
		}

		lua_rotate(State, 0, 1);
		return 1;
	}

	template<class VectorType>
	static int __newindex(lua_State* State) {

		lua_Integer Index_Int;
		const char* Index_Str;
		size_t IndexLength_Str;
		int IndexType;

		IndexType = lua_type(State, 2);
		switch (IndexType) {
		case LUA_TSTRING:

			Index_Str = lua_tolstring(State, 2, &IndexLength_Str);

			unlikely_branch
				if (IndexLength_Str > 1) {
					luaL_error(State, "Cannot set index of Vector: Invalid index: ", Index_Str);
				}

			unlikely_branch
				if (Index_Str[0] < 'X' || Index_Str[0] > 'X' + VectorType::ComponentCount - 1) {
					luaL_error(State, "Cannot set component of Vector: Invalid component index: ", Index_Str);
				}

			(*LuaHelper::ToTypename<VectorType*>(State, 1))[Index_Str[0] - 'X'] = LuaHelper::ToTypename<typename VectorType::ComponentType>(State, 3);
			//static_cast<float*>(lua_touserdata(State, 1))[Index_Str[0] - 'X'] = luaL_checknumber(State, 3);
			return 0;

		case LUA_TNUMBER:

			Index_Int = lua_tointeger(State, 2);

			unlikely_branch
				if (Index_Int < 0 || Index_Int > 2) {
					luaL_error(State, "Cannot set component of Vector3: Numerical index out of range (", lua_tostring(State, 2), ").");
				}

			(*LuaHelper::ToTypename<VectorType*>(State, 1))[Index_Int] = LuaHelper::ToTypename<typename VectorType::ComponentType>(State, 3);
			//static_cast<float*>(lua_touserdata(State, 1))[Index_Int] = luaL_checknumber(State, 3);
			return 0;

		default:
			luaL_error(State, "Cannot set component of Vector3: Invalid index type '", lua_typename(State, IndexType), "'.");
		}

		return 0;
	}

	template<class VectorType>
	static int __add(lua_State* State) {
		VectorType* Result;

		lua_settop(State, 2);
		Result = static_cast<VectorType*>(lua_newuserdata(State, sizeof(VectorType)));
		*Result = *static_cast<VectorType*>(lua_touserdata(State, 1));

		if (lua_isuserdata(State, 2)) {

			lua_getmetatable(State, 1);
			lua_getmetatable(State, 2);

			unlikely_branch
				if (!lua_compare(State, -1, -2, LUA_OPEQ)) {
					luaL_error(State, "");
				}
			*Result += *static_cast<VectorType*>(lua_touserdata(State, 2));
			//goto ReturnResult;

		} else {
			*Result += LuaHelper::ToTypename<typename VectorType::ComponentType>(State, 2);
		}

	ReturnResult:
		lua_rotate(State, 0, 1);
		return 1;
	}

	template<class VectorType>
	static int __sub(lua_State* State) {
		VectorType* Result;

		lua_settop(State, 2);
		Result = static_cast<VectorType*>(lua_newuserdata(State, sizeof(VectorType)));
		*Result = *static_cast<VectorType*>(lua_touserdata(State, 1));

		if (lua_isuserdata(State, 2)) {

			lua_getmetatable(State, 1);
			lua_getmetatable(State, 2);

			unlikely_branch
				if (!lua_compare(State, -1, -2, LUA_OPEQ)) {
					luaL_error(State, "");
				}
			*Result -= *static_cast<VectorType*>(lua_touserdata(State, 2));

		} else {
			*Result -= LuaHelper::ToTypename<typename VectorType::ComponentType>(State, 2);
		}
		lua_rotate(State, 0, 1);
		return 1;
	}

	template<class VectorType>
	static int __unm(lua_State* State) {
		VectorType* Result = static_cast<VectorType*>(lua_newuserdata(State, sizeof(VectorType)));
		*Result = static_cast<VectorType*>(lua_touserdata(State, 1))->operator-();

		lua_rotate(State, 0, 1);
		return 1;
	}

	template<class VectorType>
	static int __mul(lua_State* State) {
		VectorType* Result;

		lua_settop(State, 2);
		Result = static_cast<VectorType*>(lua_newuserdata(State, sizeof(VectorType)));
		*Result = *static_cast<VectorType*>(lua_touserdata(State, 1));

		if (lua_isuserdata(State, 2)) {

			lua_getmetatable(State, 1);
			lua_getmetatable(State, 2);

			unlikely_branch
				if (!lua_compare(State, -1, -2, LUA_OPEQ)) {
					luaL_error(State, "");
				}
			*Result *= LuaHelper::GetUserdata<VectorType>(State, 2);

		} else {
			*Result *= LuaHelper::ToTypename<typename VectorType::ComponentType>(State, 2);
		}
		lua_rotate(State, 0, 1);
		return 1;
	}

	template<class VectorType>
	static int __div(lua_State* State) {
		VectorType*
			Result;

		lua_settop(State, 2);
		Result = static_cast<VectorType*>(lua_newuserdata(State, sizeof(VectorType)));
		*Result = *static_cast<VectorType*>(lua_touserdata(State, 1));

		if (lua_isuserdata(State, 2)) {

			lua_getmetatable(State, 1);
			lua_getmetatable(State, 2);

			unlikely_branch
				if (!lua_compare(State, -1, -2, LUA_OPEQ)) {
					luaL_error(State, "");
				}
			*Result /= *static_cast<VectorType*>(lua_touserdata(State, 2));

		} else {
			*Result /= LuaHelper::ToTypename<typename VectorType::ComponentType>(State, 2);
		}
		lua_rotate(State, 0, 1);
		return 1;
	}

	template<class VectorType>
	static int __mod(lua_State* State) {
		VectorType* Result;

		lua_settop(State, 2);
		Result = static_cast<VectorType*>(lua_newuserdata(State, sizeof(VectorType)));
		*Result = *static_cast<VectorType*>(lua_touserdata(State, 1));

		if (lua_isuserdata(State, 2)) {

			lua_getmetatable(State, 1);
			lua_getmetatable(State, 2);

			unlikely_branch
				if (!lua_compare(State, -1, -2, LUA_OPEQ)) {
					luaL_error(State, "");
				}
			*Result %= *static_cast<VectorType*>(lua_touserdata(State, 2));

		} else {
			*Result %= LuaHelper::ToTypename<typename VectorType::ComponentType>(State, 2);
		}
		lua_rotate(State, 0, 1);
		return 1;
	}


	template<class VectorType>
	static int __eq(lua_State* State) {

		int Result = false;

		if (lua_isuserdata(State, 2)) {

			lua_getmetatable(State, 1);
			lua_getmetatable(State, 2);

			if (lua_compare(State, -1, -2, LUA_OPEQ)) {
				Result = LuaHelper::GetUserdata<VectorType>(State, 1) == LuaHelper::GetUserdata<VectorType>(State, 2);
			}
		}

		lua_settop(State, 0);
		lua_pushboolean(State, Result);
		return 1;
	}


	template<class VectorType>
	static int __Magnitude(lua_State* State) {

		LuaHelper::Push<typename VectorType::ComponentType>(State, LuaHelper::GetUserdata<VectorType>(State, 1).Magnitude());
		lua_rotate(State, 0, 1);
		return 1;
	}

	template<class VectorType>
	static int __Dot(lua_State* State) {

		const VectorType* B = static_cast<VectorType*>(lua_touserdata(State, 2));

		lua_getmetatable(State, 1);
		lua_getmetatable(State, 2);

		unlikely_branch
		if (B == NULL || !lua_compare(State, -1, -2, LUA_OPEQ)) {
			luaL_error(State, "Attempted to get the Dot product with an unexpected 2nd argument.");
		}

		LuaHelper::Push<typename VectorType::ComponentType>(State, LuaHelper::GetUserdata<VectorType>(State, 1).Dot(*B));
		lua_rotate(State, 0, 1);
		return 1;
	}

	template<class VectorType>
	static int __Normalize(lua_State* State) {

		VectorType* A = static_cast<VectorType*>(lua_touserdata(State, 1));
		*A = A->Normalize();
		lua_settop(State, 1);
		return 1;
	}


	template<class VectorType>
	static inline void SetupVector(lua_State* State, const char* VectorName) {

		LuaHelper::StackTableReference VectorTable(State, 0, 1);
		LuaHelper::StackTableReference VectorMetatable(State, VectorName);

		VectorMetatable.SetKeyClosure(State, ::__tostring<VectorType>, "__tostring");

		VectorMetatable.SetKeyClosure(State, ::__index<VectorType>, "__index");
		VectorMetatable.SetKeyClosure(State, ::__newindex<VectorType>, "__newindex");

		VectorMetatable.SetKeyClosure(State, ::__add<VectorType>, "__add");
		VectorMetatable.SetKeyClosure(State, ::__sub<VectorType>, "__sub");
		VectorMetatable.SetKeyClosure(State, ::__unm<VectorType>, "__unm");

		VectorMetatable.SetKeyClosure(State, ::__mul<VectorType>, "__mul");
		VectorMetatable.SetKeyClosure(State, ::__div<VectorType>, "__div");
		VectorMetatable.SetKeyClosure(State, ::__mod<VectorType>, "__mod");

		VectorMetatable.SetKeyClosure(State, ::__eq<VectorType>, "__eq");


		VectorMetatable.SetKeyClosure(State, ::__Magnitude<VectorType>, "Magnitude");
		VectorMetatable.SetKeyClosure(State, ::__Dot<VectorType>, "Dot");

		if CONSTEXPR_IF(std::is_floating_point_v<typename VectorType::ComponentType>) {
			VectorMetatable.SetKeyClosure(State, ::__Normalize<VectorType>, "Normalize");
		}


		VectorTable.SetKeyClosure(State, ::__new<VectorType>, "new", 1);

		lua_setglobal(State, VectorName);
	}
};


namespace Game::Lua::CLibraries::Vector {

	inline void Init(lua_State* State) {

		::SetupVector<Math::Vector2>(State,  "Vector2");
		::SetupVector<Math::IVector2>(State, "IVector2");
		::SetupVector<Math::UVector2>(State, "UVector2");

		::SetupVector<Math::Vector3>(State, "Vector3");
		::SetupVector<Math::IVector3>(State, "IVector3");
		::SetupVector<Math::UVector3>(State, "UVector3");

		::SetupVector<Math::Vector4>(State, "Vector4");
		::SetupVector<Math::IVector4>(State, "IVector4");
		::SetupVector<Math::UVector4>(State, "UVector4");
	}
}