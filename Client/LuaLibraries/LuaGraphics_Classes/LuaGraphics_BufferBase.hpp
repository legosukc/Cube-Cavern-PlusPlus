#pragma once

#include "LuaGraphics_GLObjectBase.hpp"


#include <SDL2/SDL_stdinc.h>

#include <lua-5.5.0/lua.hpp>

#include <glad/glad.h>


#include "../../../FunctionHeaders/LuaHelper.hpp"


namespace Game::Lua::CLibraries::Graphics {
	
	namespace Classes {
		struct BufferBase : Game::Lua::CLibraries::Graphics::Classes::GLObjectBase {
			static void InitMetatable(lua_State* State, const char* MetatableName);

			GLenum BufferType;

			static int Bind(lua_State* State);

			static int __gc(lua_State* State);
			static int __eq(lua_State* State);
		};
	}
	
	namespace BufferBase {
		template<GLenum BufferType>
		static inline void Init(lua_State* State, const char* Name);

		template<GLenum BufferType>
		static int Create(lua_State* State);

		template<GLenum BufferType>
		static int Unbind(lua_State* State);

		template<GLenum BufferType>
		static int BufferData(lua_State* State);
	}
}



template<GLenum BufferType>
void Game::Lua::CLibraries::Graphics::BufferBase::Init(lua_State* State, const char* Name) {

	LuaHelper::StackTableReference BufferBaseTable(State, 0, 2);

	lua_pushstring(State, Name);
	BufferBaseTable.SetKeyClosure(State, BufferBase::Create<BufferType>, "Create", 1);

	BufferBaseTable.SetKey(State, BufferBase::Unbind<BufferType>, "Unbind");

	lua_setfield(State, BufferBaseTable.GetStackIndex() - 1, Name);
}


template<GLenum BufferType>
int Game::Lua::CLibraries::Graphics::BufferBase::Create(lua_State* State) {

	lua_settop(State, 0);

	Classes::BufferBase* BufferUD = static_cast<Classes::BufferBase*>(lua_newuserdata(State, sizeof(Classes::BufferBase)));
	BufferUD->BufferType = BufferType;
	glGenBuffers(1, &BufferUD->GLObject);

	luaL_setmetatable(State, lua_tostring(State, lua_upvalueindex(1)));

	return 1;
}

template<GLenum BufferType>
int Game::Lua::CLibraries::Graphics::BufferBase::Unbind(lua_State* State) {
	glBindBuffer(BufferType, 0);
	return 0;
}


template<GLenum BufferType>
int Game::Lua::CLibraries::Graphics::BufferBase::BufferData(lua_State* State) {
	
	luaL_checkudata(State, 1, "Buffer");
}




void Game::Lua::CLibraries::Graphics::Classes::BufferBase::InitMetatable(lua_State* State, const char* MetatableName) {
	LuaHelper::StackTableReference Metatable = GLObjectBase::InitMetatable(State, MetatableName);

	Metatable.SetKeyClosure(State, BufferBase::Bind, "Bind");
}

int Game::Lua::CLibraries::Graphics::Classes::BufferBase::Bind(lua_State* State) {
	const BufferBase* const BufferUD = static_cast<BufferBase*>(lua_touserdata(State, 1));
	glBindBuffer(BufferUD->BufferType, BufferUD->GLObject);
	return 0;
}
int Game::Lua::CLibraries::Graphics::Classes::BufferBase::__gc(lua_State* State) {
	glDeleteBuffers(1, &static_cast<BufferBase*>(lua_touserdata(State, 1))->GLObject);
	return 0;
}

int Game::Lua::CLibraries::Graphics::Classes::BufferBase::__eq(lua_State* State) {

	const bool Result = lua_isuserdata(State, 2)
		&& *static_cast<Uint64*>(lua_touserdata(State, 1))
		== *static_cast<Uint64*>(lua_touserdata(State, 2));

	lua_settop(State, 0);
	lua_pushboolean(State, static_cast<int>(Result));
	return 1;
}