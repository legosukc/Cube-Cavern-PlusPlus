#pragma once

#include "../../define.h"

#include <SDL3/SDL_opengl.h>

#include <lua.hpp>

#include "../../FunctionHeaders/LuaHelper.hpp"


namespace Game::Lua::CLibraries::Graphics::Classes {

	struct GLObjectBase {
		static LuaHelper::StackTableReference InitMetatable(lua_State* State, const char* MetatableName);

		GLuint GLObject;

		static int __gc(lua_State* State);
	};
}


LuaHelper::StackTableReference Game::Lua::CLibraries::Graphics::Classes::GLObjectBase::InitMetatable(lua_State* State, const char* MetatableName) {

	LuaHelper::StackTableReference Metatable(State, MetatableName);

	Metatable.SetKey(State, GLObjectBase::__gc, "__gc");
	
	Metatable.PushReference(State);
	lua_setfield(State, Metatable.GetStackIndex(), "__index");

	return Metatable;
}

int Game::Lua::CLibraries::Graphics::Classes::GLObjectBase::__gc(lua_State* State) {
	return 0;
}