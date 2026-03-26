
#include "LuaVector.hpp"

#include <iostream>
#include <typeinfo>

#include "define.h"

#include <lua-5.4.2/lua.hpp>

#include "Vector3.hpp"


int Game::Lua::CLibraries::Vector::Vector3::__new(lua_State* State) {

	lua_settop(State, 3);

	Math::Vector3* NewVector = reinterpret_cast<Math::Vector3*>(lua_newuserdata(State, sizeof(Math::Vector3)));
	luaL_setmetatable(State, "Vector3");

	if (lua_isnoneornil(State, 1)) {
		NewVector->X = 0.f;
	} else if (lua_isnumber(State, 1)) {
		NewVector->X = lua_tonumber(State, 1);

	unlikely_branch
	} else {
		luaL_argerror(State, 1, "Expected type 'number' or 'nil' for parameter 1");
	}

	if (lua_isnoneornil(State, 2)) {
		NewVector->Y = 0.f;
	} else if (lua_isnumber(State, 2)) {
		NewVector->Y = lua_tonumber(State, 2);

	unlikely_branch
	} else {
		luaL_argerror(State, 2, "Expected type 'number' or 'nil' for parameter 2");
	}

	if (lua_isnoneornil(State, 3)) {
		NewVector->Z = 0.f;
	} else if (lua_isnumber(State, 3)) {
		NewVector->Z = lua_tonumber(State, 3);

	unlikely_branch
	} else {
		luaL_argerror(State, 3, "Expected type 'number' or 'nil' for parameter 3");
	}
	
	lua_rotate(State, 0, 1);
	return 1;
}


int Game::Lua::CLibraries::Vector::Vector3::__index(lua_State* State) {

	if (const char FirstIndexChar = luaL_checklstring(State, 2, NULL)[0]; FirstIndexChar >= 'X' && FirstIndexChar <= 'Z') {
		lua_pushnumber(State, static_cast<lua_Number>(
				reinterpret_cast<float*>(lua_touserdata(State, 1))[FirstIndexChar - 'X']
			)
		);
	} else {
		lua_getmetatable(State, 1);
		lua_pushvalue(State, 2);
		lua_gettable(State, -2);
	}

	lua_rotate(State, 0, 1);
	return 1;
}

int Game::Lua::CLibraries::Vector::Vector3::__newindex(lua_State* State) {

	likely_branch
	if (const char Index = luaL_checklstring(State, 2, NULL)[0]; Index >= 'X' && Index <= 'Z') {
		reinterpret_cast<float*>(lua_touserdata(State, 1))[Index - 'X'] = static_cast<float>(luaL_checknumber(State, 3));

	unlikely_branch
	} else {
		luaL_argerror(State, 2, "Index assignment argument has to be a vector component. (X, Y, Z)");
	}

	return 0;
}

int Game::Lua::CLibraries::Vector::Vector3::__add(lua_State* State) {

	Math::Vector3* A, *BVec, Result;
	A = reinterpret_cast<Math::Vector3*>(lua_touserdata(State, 1));

	if ((BVec = reinterpret_cast<Math::Vector3*>(luaL_checkudata(State, 2, "Vector3")))) {
		Result = *A + *BVec;
	} else if (lua_isnumber(State, 2)) {
		Result = *A + static_cast<float>(lua_tonumber(State, 2));

	unlikely_branch
	} else {
		luaL_error(State, "Unexpected type while adding to a Vector3. Expected 'number' or 'Vector3', got ", luaL_typename(State, 2));
	}

	lua_settop(State, 0);
	*reinterpret_cast<Math::Vector3*>(lua_newuserdata(State, sizeof(Math::Vector3))) = Result;
	return 1;
}

int Game::Lua::CLibraries::Vector::Vector3::__sub(lua_State* State) {

	Math::Vector3* A, *BVec, Result;
	A = reinterpret_cast<Math::Vector3*>(lua_touserdata(State, 1));

	if ((BVec = reinterpret_cast<Math::Vector3*>(luaL_checkudata(State, 2, "Vector3")))) {
		Result = *A - *BVec;
	} else if (lua_isnumber(State, 2)) {
		Result = *A - static_cast<float>(lua_tonumber(State, 2));

		unlikely_branch
	} else {
		luaL_error(State, "Unexpected type while subtracting to a Vector3. Expected 'number' or 'Vector3', got ", luaL_typename(State, 2));
	}

	lua_settop(State, 0);
	*reinterpret_cast<Math::Vector3*>(lua_newuserdata(State, sizeof(Math::Vector3))) = Result;
	return 1;
}

int Game::Lua::CLibraries::Vector::Vector3::__mul(lua_State* State) {

	Math::Vector3* A, * BVec, Result;
	A = reinterpret_cast<Math::Vector3*>(lua_touserdata(State, 1));

	if ((BVec = reinterpret_cast<Math::Vector3*>(luaL_checkudata(State, 2, "Vector3")))) {
		Result = *A * *BVec;
	} else if (lua_isnumber(State, 2)) {
		Result = *A * static_cast<float>(lua_tonumber(State, 2));

		unlikely_branch
	} else {
		luaL_error(State, "Unexpected type while multipling to a Vector3. Expected 'number' or 'Vector3', got ", luaL_typename(State, 2));
	}

	lua_settop(State, 0);
	*reinterpret_cast<Math::Vector3*>(lua_newuserdata(State, sizeof(Math::Vector3))) = Result;
	return 1;
}

int Game::Lua::CLibraries::Vector::Vector3::__div(lua_State* State) {

	Math::Vector3* A, * BVec, Result;
	A = reinterpret_cast<Math::Vector3*>(lua_touserdata(State, 1));

	if ((BVec = reinterpret_cast<Math::Vector3*>(luaL_checkudata(State, 2, "Vector3")))) {
		Result = *A / *BVec;
	} else if (lua_isnumber(State, 2)) {
		Result = *A / static_cast<float>(lua_tonumber(State, 2));

		unlikely_branch
	} else {
		luaL_error(State, "Unexpected type while dividing to a Vector3. Expected 'number' or 'Vector3', got ", luaL_typename(State, 2));
	}

	lua_settop(State, 0);
	*reinterpret_cast<Math::Vector3*>(lua_newuserdata(State, sizeof(Math::Vector3))) = Result;
	return 1;
}




void Game::Lua::CLibraries::Vector::Init(lua_State* State) {

	luaL_newmetatable(State, "Vector3");

	lua_pushcfunction(State, Vector3::__index);
	lua_setfield(State, -2, "__index");

	lua_pushcfunction(State, Vector3::__newindex);
	lua_setfield(State, -2, "__newindex");

	lua_pushcfunction(State, Vector3::__add);
	lua_setfield(State, -2, "__add");

	lua_pushcfunction(State, Vector3::__sub);
	lua_setfield(State, -2, "__sub");

	lua_pushcfunction(State, Vector3::__mul);
	lua_setfield(State, -2, "__mul");

	lua_pushcfunction(State, Vector3::__div);
	lua_setfield(State, -2, "__div");


	lua_createtable(State, 0, 1); // Vector3

	lua_pushcfunction(State, Vector3::__new);
	lua_setfield(State, -2, "new");

	lua_setglobal(State, "Vector3");
}