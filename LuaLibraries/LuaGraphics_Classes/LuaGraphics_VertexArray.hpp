#pragma once

#include "LuaGraphics_GLObjectBase.hpp"


#include <glad/glad.h>

#include <lua-5.5.0/lua.hpp>

#include "../../FunctionHeaders/LuaHelper.hpp"


namespace Game::Lua::CLibraries::Graphics {

	namespace Classes {
		struct VertexArray : Game::Lua::CLibraries::Graphics::Classes::GLObjectBase {
			static int Bind(lua_State* State);

			//static int __eq(lua_State* State);
			static int __gc(lua_State* State);
		};
	}

	namespace VertexArray {
		static inline void Init(lua_State* State, LuaHelper::StackTableReference& GraphicsTable);

		static int __new(lua_State* State);


		static int EnableAttribute(lua_State* State) {
			glEnableVertexAttribArray(static_cast<GLuint>(luaL_checkinteger(State, 1)));
			return 0;
		}

		static int DisableAttribute(lua_State* State) {
			glDisableVertexAttribArray(static_cast<GLuint>(luaL_checkinteger(State, 1)));
			return 0;
		}

		static int SetAttributePointer(lua_State* State) {
			glVertexAttribPointer(
				static_cast<GLuint>(luaL_checkinteger(State, 1)),		// Index
				static_cast<GLint>(luaL_checkinteger(State, 2)),		// Size
				static_cast<GLenum>(luaL_checkinteger(State, 3)),		// Type
				static_cast<GLboolean>(lua_toboolean(State, 4)),		// Normalized
				static_cast<GLsizei>(luaL_checkinteger(State, 5)),		// Stride Size
				reinterpret_cast<void*>(luaL_optinteger(State, 6, 0))	// Offset
			);
			return 0;
		}

		static int Unbind(lua_State* State) {
			glBindVertexArray(0);
			return 0;
		}
	}
}


void Game::Lua::CLibraries::Graphics::VertexArray::Init(lua_State* State, LuaHelper::StackTableReference& GraphicsTable) {

	LuaHelper::StackTableReference VertexArrayMetatable(State, "VertexArray");

	VertexArrayMetatable.PushReference(State);
	VertexArrayMetatable.SetKeyClosure(State, Classes::VertexArray::Bind, "Bind", 1);

	VertexArrayMetatable.SetKeyClosure(State, Classes::VertexArray::__gc, "__gc");
	//VertexArrayMetatable.SetKeyClosure(State, Classes::VertexArray::__eq, "__eq");

	VertexArrayMetatable.PushReference(State);
	lua_setfield(State, VertexArrayMetatable.GetStackIndex(), "__index");

	lua_settop(State, VertexArrayMetatable.GetStackIndex() - 1);


	LuaHelper::StackTableReference VertexArrayTable(State, 0, 2); // OpenGL.VertexArray

	VertexArrayTable.SetKeyClosure(State, VertexArray::__new, "new");

	VertexArrayTable.SetKeyClosure(State, VertexArray::EnableAttribute, "EnableAttribute");
	VertexArrayTable.SetKeyClosure(State, VertexArray::DisableAttribute, "DisableAttribute");

	VertexArrayTable.SetKeyClosure(State, VertexArray::SetAttributePointer, "SetAttributePointer");

	VertexArrayTable.SetKeyClosure(State, VertexArray::Unbind, "Unbind");

	lua_setfield(State, GraphicsTable.GetStackIndex(), "VertexArray");
}

int Game::Lua::CLibraries::Graphics::VertexArray::__new(lua_State* State) {

	Classes::VertexArray* VertexArrayUD = static_cast<Classes::VertexArray*>(lua_newuserdata(State, sizeof(Classes::VertexArray)));
	glGenVertexArrays(1, &VertexArrayUD->GLObject);
	luaL_setmetatable(State, "VertexArray");
	return 1;
}



int Game::Lua::CLibraries::Graphics::Classes::VertexArray::Bind(lua_State* State) {
	glBindVertexArray(static_cast<Classes::VertexArray*>(LuaHelper::CheckMetatable(State, 1, lua_upvalueindex(1)))->GLObject);
	return 0;
}
/*
int Game::Lua::CLibraries::Graphics::Classes::VertexArray::__eq(lua_State* State) {

	bool Result;
	Classes::VertexArray* A, *B;
	
	A = static_cast<Classes::VertexArray*>(luaL_testudata(State, 1, "VertexArray"));
	B = static_cast<Classes::VertexArray*>(luaL_testudata(State, 2, "VertexArray"));

	if (A == NULL || B == NULL) {
		Result = false;
	} else {
		Result = A->GLObject == B->GLObject;
	}

	lua_settop(State, 0);
	lua_pushboolean(State, static_cast<int>(Result));
	return 1;
}*/

int Game::Lua::CLibraries::Graphics::Classes::VertexArray::__gc(lua_State* State) {
	glDeleteVertexArrays(1, &static_cast<Classes::VertexArray*>(luaL_checkudata(State, 1, "VertexArray"))->GLObject);
	return 0;
}
