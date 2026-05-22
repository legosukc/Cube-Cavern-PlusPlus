#pragma once

#include <lua-5.5.0/lua.hpp>

#include "../FunctionHeaders/LuaHelper.hpp"


#include "../MathClasses/Mat4.hpp"
#include "../FunctionHeaders/MathTransform.hpp"


namespace {
	namespace _LuaMatrix {

		template<class MatrixType>
		static int __new(lua_State* State) {

			MatrixType* NewMatrix;
			typename MatrixType::ColumnType* Row1;

			NewMatrix = static_cast<MatrixType*>(lua_newuserdata(State, sizeof(MatrixType)));

			lua_rotate(State, 1, 1);

			if (Row1 = static_cast<typename MatrixType::ColumnType*>(LuaHelper::TestMetatable(State, 1, lua_upvalueindex(1))); Row1 == NULL) {

				lua_settop(State, (MatrixType::RowCount * MatrixType::ColumnCount) + 1);

				for (int i = 2; i <= MatrixType::RowCount * MatrixType::ColumnCount; ++i) {
					reinterpret_cast<typename MatrixType::ColumnType::ComponentType*>(NewMatrix)[i - 2] = luaL_optnumber(State, i, 0.0);
				}

			} else {

				lua_settop(State, MatrixType::ColumnCount + 1);

				(*NewMatrix)[0] = *Row1;
				for (int i = 3; i <= MatrixType::ColumnCount; ++i) {
					(*NewMatrix)[i - 2] = *static_cast<typename MatrixType::ColumnType*>(LuaHelper::TestMetatable(State, i, lua_upvalueindex(1)));
				}
			}

			lua_settop(State, 1);
			return 1;
		}


		template<class MatrixType>
		static int __tostring(lua_State* State) {

			const MatrixType* MatrixUD = static_cast<MatrixType*>(LuaHelper::CheckMetatable(State, 1, lua_upvalueindex(1)));
			/*
			LuaHelper::Push<typename MatrixType::ColumnType::ComponentType>(State, (*MatrixUD)[MatrixType::ColumnType]);
			for (int i = VectorType::ComponentCount - 1; i > 0; --i) {

				//std::cout << i << std::endl;
				lua_pushliteral(State, ", ");
				LuaHelper::Push<typename MatrixType::ColumnType::ComponentType>(State, (*Vector)[i]);
			}
			lua_concat(State, VectorType::ComponentCount);

			lua_rotate(State, 1, 1);*/

			return 1;
		}

		template<class MatrixType>
		static int __index(lua_State* State) {

			lua_Integer Index;
			MatrixType* MatrixUD;

			MatrixUD = static_cast<MatrixType*>(LuaHelper::CheckMetatable(State, 1, lua_upvalueindex(1)));
			Index = luaL_checkinteger(State, 2);

			unlikely_branch
				if (Index < 0 || Index > MatrixType::ColumnCount - 1) {
					lua_getfield(State, lua_upvalueindex(1), "__name");
					luaL_error(State, "Cannot index ", lua_tostring(State, -1), ": Numerical index out of range: (", Index, ").");
				}

			LuaHelper::Push<typename MatrixType::ColumnType>(State, (*MatrixUD)[Index]);
			return 1;
		}

		template<class MatrixType>
		static int __newindex(lua_State* State) {

			lua_Integer Index;
			MatrixType* MatrixUD;

			MatrixUD = static_cast<MatrixType*>(LuaHelper::CheckMetatable(State, 1, lua_upvalueindex(1)));
			Index = luaL_checkinteger(State, 2);

			unlikely_branch
				if (Index < 0 || Index > MatrixType::ColumnCount - 1) {
					lua_getfield(State, lua_upvalueindex(1), "__name");
					luaL_error(State, "Cannot index ", lua_tostring(State, -1), ": Numerical index out of range: (", Index, ").");
				}

			(*MatrixUD)[Index] = *static_cast<typename MatrixType::ColumnType*>(LuaHelper::CheckMetatable(State, 3, lua_upvalueindex(2)));
			return 0;
		}

		template<class MatrixType>
		static int __mul(lua_State* State) {
			MatrixType* Result, * B;

			Result = static_cast<MatrixType*>(lua_newuserdata(State, sizeof(MatrixType)));

			*Result = *static_cast<MatrixType*>(LuaHelper::CheckMetatable(State, 1, lua_upvalueindex(1)))
				* *static_cast<MatrixType*>(LuaHelper::CheckMetatable(State, 2, lua_upvalueindex(1)));
			return 1;
		}


		template<class MatrixType>
		static int __eq(lua_State* State) {

			const MatrixType* A, * B;

			A = static_cast<MatrixType*>(LuaHelper::TestMetatable(State, 1, lua_upvalueindex(1)));
			B = static_cast<MatrixType*>(LuaHelper::TestMetatable(State, 2, lua_upvalueindex(1)));

			lua_pushboolean(State, !(A == NULL || B == NULL) && A->operator==(*B));
			return 1;
		}

		template<class MatrixType>
		static inline void SetupMatrix(lua_State* State, const char* VectorName, const char* MatrixName) {

			LuaHelper::StackTableReference LibraryTable, Metatable;

			LibraryTable = LuaHelper::StackTableReference(State, 0, 1);
			Metatable = LuaHelper::StackTableReference(State, MatrixName);

			for (Uint8 i = 0; i < 7; ++i) {
				//Metatable.PushReference(State);
			}

			//Metatable.PushReference(State);
			//lua_copy(State, Metatable.GetStackIndex() + 1, Metatable.GetStackIndex() + 6);

			Metatable.PushReference(State);
			Metatable.SetKeyClosure(State, ::_LuaMatrix::__tostring<MatrixType>, "__tostring", 1);


			Metatable.PushReference(State);
			Metatable.SetKeyClosure(State, ::_LuaMatrix::__index<MatrixType>, "__index", 1);

			Metatable.PushReference(State);
			luaL_getmetatable(State, VectorName);
			Metatable.SetKeyClosure(State, ::_LuaMatrix::__newindex<MatrixType>, "__newindex", 2);


			Metatable.PushReference(State);
			Metatable.SetKeyClosure(State, ::_LuaMatrix::__mul<MatrixType>, "__mul", 1);

			Metatable.PushReference(State);
			Metatable.SetKeyClosure(State, ::_LuaMatrix::__eq<MatrixType>, "__eq", 1);

			LibraryTable.SetKeyClosure(State, ::_LuaMatrix::__new<MatrixType>, "new", 1);
			LibraryTable.SetKey<lua_Integer>(State, sizeof(MatrixType), "ByteSize");

			lua_setglobal(State, MatrixName);
		}
	}



	namespace _Lua_Mat4 {

		static int _LookAt(lua_State* State) {

			Math::Mat4* Result = static_cast<Math::Mat4*>(lua_newuserdata(State, sizeof(Math::Mat4)));
			luaL_setmetatable(State, "Mat4");

			*Result = Math::Transform::LookAt(
				*static_cast<Math::Vector3*>(luaL_checkudata(State, 1, "Vector3")),		// Eye
				*static_cast<Math::Vector3*>(luaL_checkudata(State, 2, "Vector3")),		// Center
				*static_cast<Math::Vector3*>(luaL_checkudata(State, 3, "Vector3"))		// Up
			);

			return 1;
		}

		static int _Perspective(lua_State* State) {

			Math::Mat4* Result = static_cast<Math::Mat4*>(lua_newuserdata(State, sizeof(Math::Mat4)));
			luaL_setmetatable(State, "Mat4");

			*Result = Math::Transform::Perspective(
				static_cast<float>(luaL_checknumber(State, 1)),		// FOV
				static_cast<float>(luaL_checknumber(State, 2)),		// Aspect Ratio
				static_cast<float>(luaL_checknumber(State, 3)),		// zNear
				static_cast<float>(luaL_checknumber(State, 4))		// zFar
				//*static_cast<googie*>(luaL_checkudata(State, 5, "googie"))	// zKevin (haha what a funny joke)
			);

			return 1;
		}
	}
};


namespace Game::Lua::CLibraries::Matrix {

	inline void Init(lua_State* State) {

		//::SetupMatrix<Math::Mat2>(State, "Vector2", "Mat2");
		//::SetupMatrix<Math::Mat3>(State, "Vector3", "Mat3");
		::_LuaMatrix::SetupMatrix<Math::Mat4>(State, "Vector4", "Mat4");

		lua_getglobal(State, "Mat4");

		lua_pushcfunction(State, ::_Lua_Mat4::_LookAt);
		lua_setfield(State, -2, "LookAt");

		lua_pushcfunction(State, ::_Lua_Mat4::_Perspective);
		lua_setfield(State, -2, "Perspective");

		lua_pop(State, 1);
	}
}