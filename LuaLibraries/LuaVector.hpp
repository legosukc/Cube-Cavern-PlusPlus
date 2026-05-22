#pragma once

#include <lua-5.5.0/lua.hpp>

#include <type_traits>

#include "../FunctionHeaders/LuaHelper.hpp"


#include "../MathClasses/Vector2.hpp"
#include "../MathClasses/Vector3.hpp"
#include "../MathClasses/Vector4.hpp"

namespace {
	namespace _LuaVector {

		template<class VectorType>
		static int __new(lua_State* State) {

			lua_settop(State, VectorType::ComponentCount);

			VectorType* const NewVector = static_cast<VectorType*>(lua_newuserdata(State, sizeof(VectorType)));
			lua_pushvalue(State, lua_upvalueindex(1));
			lua_setmetatable(State, VectorType::ComponentCount + 1);

			for (int i = 1; i <= VectorType::ComponentCount; ++i) {

				if CONSTEXPR_IF(std::is_integral_v<typename VectorType::ComponentType>) {
					(*NewVector)[i] = static_cast<typename VectorType::ComponentType>(luaL_optinteger(State, i, 0));
				} else {
					(*NewVector)[i] = static_cast<typename VectorType::ComponentType>(luaL_optnumber(State, i, 0.0));
				}
			}

			return 1;
		}


		template<class VectorType>
		static int __tostring(lua_State* State) {

			const VectorType* Vector = static_cast<VectorType*>(LuaHelper::CheckMetatable(State, 1, lua_upvalueindex(1)));

			LuaHelper::Push<typename VectorType::ComponentType>(State, (*Vector)[VectorType::ComponentCount - 1]);
			for (int i = VectorType::ComponentCount - 1; i > 0; --i) {

				//std::cout << i << std::endl;
				lua_pushliteral(State, ", ");
				LuaHelper::Push<typename VectorType::ComponentType>(State, (*Vector)[i]);
			}
			lua_concat(State, VectorType::ComponentCount);

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
			VectorType* VectorUD;

			lua_settop(State, 2);

			VectorUD = static_cast<VectorType*>(LuaHelper::CheckMetatable(State, 1, lua_upvalueindex(1)));

			IndexType = lua_type(State, 2);
			switch (IndexType) {
			case LUA_TSTRING:

				Index_Str = lua_tolstring(State, 2, &IndexLength_Str);
				if (IndexLength_Str == 1) {

					unlikely_branch
						if (Index_Str[0] >= 'X' && Index_Str[0] <= 'X' + VectorType::ComponentCount - 1) {
							luaL_error(State, "Cannot index Vector: Invalid component index: ", Index_Str);
						}

					LuaHelper::Push<typename VectorType::ComponentType>(State, (*VectorUD)[Index_Str[0] - 'X']);

				} else {

					//lua_getmetatable(State, 1);
					lua_pushvalue(State, 2);
					lua_gettable(State, lua_upvalueindex(1));
				}
				break;

			case LUA_TNUMBER:

				Index_Int = lua_tointeger(State, 2);

				unlikely_branch
					if (Index_Int < 0 || Index_Int > VectorType::ComponentCount - 1) {
						luaL_error(State, "Cannot index Vector: Numerical index out of range: (", Index_Int, ").");
					}

				LuaHelper::Push<typename VectorType::ComponentType>(State, (*VectorUD)[Index_Int]);
				break;

			default:
				luaL_error(State, "Cannot index Vector with type '", lua_typename(State, IndexType), "'. Expected 'string' or 'integer'.");
			}

			return 1;
		}

		template<class VectorType>
		static int __newindex(lua_State* State) {

			lua_Integer Index_Int;
			const char* Index_Str;
			size_t IndexLength_Str;
			int IndexType;
			VectorType* VectorUD;

			lua_settop(State, 3);

			VectorUD = static_cast<VectorType*>(LuaHelper::CheckMetatable(State, 1, lua_upvalueindex(1)));

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

				(*VectorUD)[Index_Str[0] - 'X'] = LuaHelper::ToTypename<typename VectorType::ComponentType>(State, 3);
				//static_cast<float*>(lua_touserdata(State, 1))[Index_Str[0] - 'X'] = luaL_checknumber(State, 3);
				return 0;

			case LUA_TNUMBER:

				Index_Int = lua_tointeger(State, 2);

				unlikely_branch
					if (Index_Int < 0 || Index_Int > 2) {
						luaL_error(State, "Cannot set component of Vector3: Numerical index out of range (", lua_tostring(State, 2), ").");
					}

				(*VectorUD)[Index_Int] = LuaHelper::ToTypename<typename VectorType::ComponentType>(State, 3);
				//static_cast<float*>(lua_touserdata(State, 1))[Index_Int] = luaL_checknumber(State, 3);
				return 0;

			default:
				luaL_error(State, "Cannot set component of Vector3: Invalid index type '", lua_typename(State, IndexType), "'.");
			}

			return 0;
		}

		template<class VectorType>
		static int __add(lua_State* State) {
			VectorType* Result, * B;

			lua_settop(State, 2);
			lua_pushvalue(State, lua_upvalueindex(1));

			Result = static_cast<VectorType*>(lua_newuserdata(State, sizeof(VectorType)));
			lua_pushvalue(State, 3);
			lua_setmetatable(State, 4);

			*Result = *static_cast<VectorType*>(LuaHelper::CheckMetatable(State, 1, 3));

			if (B = static_cast<VectorType*>(LuaHelper::TestMetatable(State, 2, 3)); B != NULL) {
				Result->operator+=(*B);
			} else {
				Result->operator+=(LuaHelper::ToTypename<typename VectorType::ComponentType>(State, 2));
			}

			return 1;
		}

		template<class VectorType>
		static int __sub(lua_State* State) {
			VectorType* Result, * B;

			lua_settop(State, 2);
			lua_pushvalue(State, lua_upvalueindex(1));

			Result = static_cast<VectorType*>(lua_newuserdata(State, sizeof(VectorType)));
			lua_pushvalue(State, 3);
			lua_setmetatable(State, 4);

			*Result = *static_cast<VectorType*>(LuaHelper::CheckMetatable(State, 1, 3));

			if (B = static_cast<VectorType*>(LuaHelper::TestMetatable(State, 2, 3)); B != NULL) {
				*Result -= *B;
			} else {
				*Result -= LuaHelper::ToTypename<typename VectorType::ComponentType>(State, 2);
			}

			return 1;
		}

		template<class VectorType>
		static int __unm(lua_State* State) {
			
			lua_pushvalue(State, lua_upvalueindex(1));
			VectorType* Result = static_cast<VectorType*>(lua_newuserdata(State, sizeof(VectorType)));
			lua_pushvalue(State, -2);
			lua_setmetatable(State, -2);

			*Result = static_cast<VectorType*>(LuaHelper::CheckMetatable(State, 1, -2))->operator-();
			return 1;
		}

		template<class VectorType>
		static int __mul(lua_State* State) {
			VectorType* Result, * B;

			lua_settop(State, 2);
			lua_pushvalue(State, lua_upvalueindex(1));

			Result = static_cast<VectorType*>(lua_newuserdata(State, sizeof(VectorType)));
			lua_pushvalue(State, 3);
			lua_setmetatable(State, 4);

			*Result = *static_cast<VectorType*>(LuaHelper::CheckMetatable(State, 1, 3));

			if (B = static_cast<VectorType*>(LuaHelper::TestMetatable(State, 2, 3)); B != NULL) {
				*Result *= *B;
			} else {
				*Result *= LuaHelper::ToTypename<typename VectorType::ComponentType>(State, 2);
			}

			return 1;
		}

		template<class VectorType>
		static int __div(lua_State* State) {
			VectorType* Result, * B;

			lua_settop(State, 2);
			lua_pushvalue(State, lua_upvalueindex(1));

			Result = static_cast<VectorType*>(lua_newuserdata(State, sizeof(VectorType)));
			lua_pushvalue(State, 3);
			lua_setmetatable(State, 4);

			*Result = *static_cast<VectorType*>(LuaHelper::CheckMetatable(State, 1, 3));

			if (B = static_cast<VectorType*>(LuaHelper::TestMetatable(State, 2, 3)); B != NULL) {
				*Result /= *B;
			} else {
				*Result /= LuaHelper::ToTypename<typename VectorType::ComponentType>(State, 2);
			}

			return 1;
		}

		template<class VectorType>
		static int __idiv(lua_State* State) {
			VectorType* Result, * B;

			lua_settop(State, 2);
			lua_pushvalue(State, lua_upvalueindex(1));

			Result = static_cast<VectorType*>(lua_newuserdata(State, sizeof(VectorType)));
			lua_pushvalue(State, 3);
			lua_setmetatable(State, 4);

			*Result = *static_cast<VectorType*>(LuaHelper::CheckMetatable(State, 1, 3));

			if (B = static_cast<VectorType*>(LuaHelper::TestMetatable(State, 2, 3)); B != NULL) {
				*Result /= *B;
			} else {
				*Result /= LuaHelper::ToTypename<typename VectorType::ComponentType>(State, 2);
			}

			return 1;
		}

		template<class VectorType>
		static int __mod(lua_State* State) {
			VectorType* Result, * B;

			lua_settop(State, 2);
			lua_pushvalue(State, lua_upvalueindex(1));

			Result = static_cast<VectorType*>(lua_newuserdata(State, sizeof(VectorType)));
			lua_pushvalue(State, 3);
			lua_setmetatable(State, 4);

			*Result = *static_cast<VectorType*>(LuaHelper::CheckMetatable(State, 1, 3));

			if (B = static_cast<VectorType*>(LuaHelper::TestMetatable(State, 2, 3)); B != NULL) {
				*Result %= *B;
			} else {
				*Result %= LuaHelper::ToTypename<typename VectorType::ComponentType>(State, 2);
			}

			return 1;
		}


		template<class VectorType>
		static int __eq(lua_State* State) {

			lua_settop(State, 2);
			lua_pushvalue(State, lua_upvalueindex(1));

			lua_pushboolean(State, *static_cast<VectorType*>(LuaHelper::CheckMetatable(State, 1, 3)) == *static_cast<VectorType*>(LuaHelper::CheckMetatable(State, 2, 3)));
			return 1;
		}


		template<class VectorType>
		static int __Magnitude(lua_State* State) {
			LuaHelper::Push<typename VectorType::ComponentType>(State, static_cast<VectorType*>(LuaHelper::CheckMetatable(State, 1, lua_upvalueindex(1)))->Magnitude());
			return 1;
		}

		template<class VectorType>
		static int __Dot(lua_State* State) {

			LuaHelper::Push<typename VectorType::ComponentType>(State,
				static_cast<VectorType*>(LuaHelper::CheckMetatable(State, 1, lua_upvalueindex(1)))->Dot(
					*static_cast<VectorType*>(LuaHelper::CheckMetatable(State, 2, lua_upvalueindex(1)))
				)
			);
			return 1;
		}

		template<class VectorType>
		static int __Normalize(lua_State* State) {

			lua_settop(State, 1);
			lua_pushvalue(State, lua_upvalueindex(1));

			*static_cast<VectorType*>(lua_newuserdata(State, sizeof(VectorType)))
				= static_cast<VectorType*>(LuaHelper::CheckMetatable(State, 1, 2))->Normalize();

			lua_pushvalue(State, 2);
			lua_setmetatable(State, 3);

			return 1;
		}


		template<class VectorType>
		static inline void SetupVector(lua_State* State, const char* VectorName) {

			LuaHelper::StackTableReference VectorTable, VectorMetatable;

			VectorTable = LuaHelper::StackTableReference(State, 0, 1);
			VectorMetatable = LuaHelper::StackTableReference(State, VectorName);

			//for (Uint8 i = 0; i < 14 + std::is_floating_point_v<typename VectorType::ComponentType>; ++i) {
				//VectorMetatable.PushReference(State);
			//}
			VectorMetatable.PushReference(State);
			VectorMetatable.SetKeyClosure(State, ::_LuaVector::__tostring<VectorType>, "__tostring", 1);


			VectorMetatable.PushReference(State);
			VectorMetatable.SetKeyClosure(State, ::_LuaVector::__add<VectorType>, "__add", 1);

			VectorMetatable.PushReference(State);
			VectorMetatable.SetKeyClosure(State, ::_LuaVector::__sub<VectorType>, "__sub", 1);

			VectorMetatable.PushReference(State);
			VectorMetatable.SetKeyClosure(State, ::_LuaVector::__unm<VectorType>, "__unm", 1);


			VectorMetatable.PushReference(State);
			VectorMetatable.SetKeyClosure(State, ::_LuaVector::__mul<VectorType>, "__mul", 1);

			VectorMetatable.PushReference(State);
			VectorMetatable.SetKeyClosure(State, ::_LuaVector::__div<VectorType>, "__div", 1);

			VectorMetatable.PushReference(State);
			VectorMetatable.SetKeyClosure(State, ::_LuaVector::__idiv<VectorType>, "__idiv", 1);

			VectorMetatable.PushReference(State);
			VectorMetatable.SetKeyClosure(State, ::_LuaVector::__mod<VectorType>, "__mod", 1);


			VectorMetatable.PushReference(State);
			VectorMetatable.SetKeyClosure(State, ::_LuaVector::__eq<VectorType>, "__eq", 1);


			VectorMetatable.PushReference(State);
			VectorMetatable.SetKeyClosure(State, ::_LuaVector::__Magnitude<VectorType>, "Magnitude", 1);

			VectorMetatable.PushReference(State);
			VectorMetatable.SetKeyClosure(State, ::_LuaVector::__Dot<VectorType>, "Dot", 1);

			if CONSTEXPR_IF(std::is_floating_point_v<typename VectorType::ComponentType>) {
				VectorMetatable.PushReference(State);
				VectorMetatable.SetKeyClosure(State, ::_LuaVector::__Normalize<VectorType>, "Normalize", 1);
			}

			VectorMetatable.PushReference(State);
			VectorMetatable.SetKeyClosure(State, ::_LuaVector::__index<VectorType>, "__index", 1);

			VectorMetatable.PushReference(State);
			VectorMetatable.SetKeyClosure(State, ::_LuaVector::__newindex<VectorType>, "__newindex", 1);


			VectorTable.SetKeyClosure(State, ::_LuaVector::__new<VectorType>, "new", 1);

			lua_setglobal(State, VectorName);
		}
	}
};


namespace Game::Lua::CLibraries::Vector {

	inline void Init(lua_State* State) {

		::_LuaVector::SetupVector<Math::Vector2>(State,  "Vector2");
		::_LuaVector::SetupVector<Math::IVector2>(State, "IVector2");
		::_LuaVector::SetupVector<Math::UVector2>(State, "UVector2");

		::_LuaVector::SetupVector<Math::Vector3>(State, "Vector3");
		::_LuaVector::SetupVector<Math::IVector3>(State, "IVector3");
		::_LuaVector::SetupVector<Math::UVector3>(State, "UVector3");

		::_LuaVector::SetupVector<Math::Vector4>(State, "Vector4");
		::_LuaVector::SetupVector<Math::IVector4>(State, "IVector4");
		::_LuaVector::SetupVector<Math::UVector4>(State, "UVector4");
	}
}