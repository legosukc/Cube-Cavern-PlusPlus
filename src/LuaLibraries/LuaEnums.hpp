#pragma once

#include <lua.hpp>

#include <SDL3/SDL_stdinc.h>


#include "../FunctionHeaders/LuaHelper.hpp"

#include "../Statistics.hpp"

#include <cstring>
#include <string>


namespace {
	static lua_Integer _NextEnumID = LUA_MININTEGER;

	static int _EnumIDsReference;
}

namespace Game::Lua::CLibraries::Enums {

	inline void Init(lua_State* State);

	static inline const char* const LibraryName = "Enums";

	namespace Classes {

		struct EnumValue {

			static inline const char* const MetatableName = "Enum";

			int ID;
			
			static int __index(lua_State* State) {
				
				lua_settop(State, 2);
				if (std::strcmp(luaL_checkstring(State, 2), "ID") == 0) {
					lua_pushinteger(State, static_cast<EnumValue*>(LuaHelper::CheckMetatable(State, 1, lua_upvalueindex(1)))->ID);
				} else {
					lua_gettable(State, lua_upvalueindex(1));
				}

				return 1;
			}
		};
	}

	static int FromID(lua_State* State) {
		lua_geti(State, ::_EnumIDsReference, luaL_checkinteger(State, 1));
		return 1;
	}

	bool RegisterEnumTable(lua_State* State, int EnumTableStackIndex, const char* EnumTableName) {
		
		lua_getglobal(State, CLibraries::Enums::LibraryName);
		lua_pushstring(State, EnumTableName);
		lua_pushvalue(State, -1);
		if (lua_rawget(State, -3) == LUA_TNIL) {

			lua_pushvalue(State, EnumTableStackIndex);
			lua_rawset(State, -3);

			return true;
		}
		return false;
	}

	Classes::EnumValue* PushNewEnum(lua_State* State, int Uservalue = 0) {
		Classes::EnumValue* NewEnum = static_cast<Classes::EnumValue*>(lua_newuserdatauv(State, sizeof(Classes::EnumValue), 1));

		lua_pushvalue(State, -1);
		lua_setiuservalue(State, -1, Uservalue);

		lua_pushvalue(State, -1);
		NewEnum->ID = luaL_ref(State, ::_EnumIDsReference);

		luaL_setmetatable(State, Classes::EnumValue::MetatableName);

		return NewEnum;
	}
}

void Game::Lua::CLibraries::Enums::Init(lua_State* State) {

	LuaHelper::StackTableReference EnumMetatable(State, CLibraries::Enums::Classes::EnumValue::MetatableName);

	EnumMetatable.PushReference(State);
	EnumMetatable.SetKeyClosure(State, Classes::EnumValue::__index, "__index", 1);
	lua_pop(State, 1);

	lua_createtable(State, 64, 0);
	::_EnumIDsReference = luaL_ref(State, LUA_REGISTRYINDEX);

	LuaHelper::StackTableReference EnumsTable(State, 0, 32);

	EnumsTable.SetKeyClosure(State, Enums::FromID, "FromID");

	LuaHelper::LockTable(State, EnumsTable.GetStackIndex());

	lua_setglobal(State, CLibraries::Enums::LibraryName);
}