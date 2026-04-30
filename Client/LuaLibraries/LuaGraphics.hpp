#pragma once

#include <lua-5.4.2/lua.hpp>

#include "LuaGraphics_Classes/LuaGraphics_GLObjectBase.hpp"
#include "LuaGraphics_Classes/LuaGraphics_BufferBase.hpp"

#include "LuaGraphics_Classes/LuaGraphics_VertexArray.hpp"
#include "LuaGraphics_Classes/LuaGraphics_VertexBuffer.hpp"
#include "LuaGraphics_Classes/LuaGraphics_ElementBuffer.hpp"

#include "LuaGraphics_Classes/LuaGraphics_Shader.hpp"
#include "LuaGraphics_Classes/LuaGraphics_Program.hpp"

#include "../../ConsoleVars.hpp"

namespace Game::ConsoleVars::ReferenceVars::Lua::Graphics {

	struct ChangeErrorCheckingCVar : Game::ConsoleVars::BaseCVar {

		virtual void Set(const char* Argument) override {
			
		}

		using VarType = void;
	};
}


namespace Game::Lua::CLibraries::Graphics {
	inline void Init(lua_State* State);

	static int DrawArrays(lua_State* State);
	static int DrawElements(lua_State* State);

	static int DrawArraysInstanced(lua_State* State);
	static int DrawElementsInstanced(lua_State* State);
}


int Game::Lua::CLibraries::Graphics::DrawArrays(lua_State* State) {
	glDrawArrays(
		static_cast<GLenum>(luaL_checkinteger(State, 1)),	// Mode
		static_cast<GLint>(luaL_optinteger(State, 2, 0)),	// First
		static_cast<GLsizei>(luaL_checkinteger(State, 3))	// Count
	);
	return 0;
}

int Game::Lua::CLibraries::Graphics::DrawElements(lua_State* State) {

	glDrawElements(
		static_cast<GLenum>(luaL_checkinteger(State, 1)),	// Mode
		static_cast<GLsizei>(luaL_checkinteger(State, 2)),	// Count
		static_cast<GLenum>(luaL_checkinteger(State, 3)),	// Type
		reinterpret_cast<const void*>(						// Offset
			luaL_optinteger(State, 4, 0)
		)
	);
	return 0;
}

int Game::Lua::CLibraries::Graphics::DrawArraysInstanced(lua_State* State) {
	glDrawArraysInstanced(
		static_cast<GLenum>(luaL_checkinteger(State, 1)),	 // Mode
		static_cast<GLint>(luaL_optinteger(State, 2, 0)),	 // First
		static_cast<GLsizei>(luaL_checkinteger(State, 3)),	 // Count
		static_cast<GLsizei>(luaL_checkinteger(State, 4))	 // Instance Count
	);
	return 0;
}

int Game::Lua::CLibraries::Graphics::DrawElementsInstanced(lua_State* State) {

	glDrawElementsInstanced(
		static_cast<GLenum>(luaL_checkinteger(State, 1)),	// Mode
		static_cast<GLsizei>(luaL_checkinteger(State, 2)),	// Count
		static_cast<GLenum>(luaL_checkinteger(State, 3)),	// Type
		reinterpret_cast<const void*>(						// Offset
			luaL_optinteger(State, 4, 0)
			),
		static_cast<GLsizei>(luaL_checkinteger(State, 5))	// Instance Count
	);
	return 0;
}




void Game::Lua::CLibraries::Graphics::Init(lua_State* State) {
	
	int AbsIndex, StartingStackIndex;
	StartingStackIndex = lua_gettop(State);

	Classes::VertexArray::InitMetatable(State);
	Classes::VertexBuffer::InitMetatable(State, "VertexBuffer");
	Classes::ElementBuffer::InitMetatable(State, "ElementBuffer");

	Classes::Program::InitMetatable(State);
	Classes::Shader::InitMetatable(State);

	// Graphics
	lua_createtable(State, 0, 6);

	// OpenGL.DrawModes
	
	lua_createtable(State, 0, 12);
	AbsIndex = lua_gettop(State);

	LuaHelper::SetKey<lua_Integer>(State, AbsIndex, GL_POINTS, "Points");

	LuaHelper::SetKey<lua_Integer>(State, AbsIndex, GL_LINE_STRIP, "LineStrip");
	LuaHelper::SetKey<lua_Integer>(State, AbsIndex, GL_LINE_LOOP, "LineLoop");
	LuaHelper::SetKey<lua_Integer>(State, AbsIndex, GL_LINES, "Lines");
	LuaHelper::SetKey<lua_Integer>(State, AbsIndex, GL_LINE_STRIP_ADJACENCY, "LineStripAdjacency");
	LuaHelper::SetKey<lua_Integer>(State, AbsIndex, GL_LINES_ADJACENCY, "LinesAdjacency");

	LuaHelper::SetKey<lua_Integer>(State, AbsIndex, GL_TRIANGLE_STRIP, "TriangleStrip");
	LuaHelper::SetKey<lua_Integer>(State, AbsIndex, GL_TRIANGLE_FAN, "TriangleFan");
	LuaHelper::SetKey<lua_Integer>(State, AbsIndex, GL_TRIANGLES, "Triangles");
	LuaHelper::SetKey<lua_Integer>(State, AbsIndex, GL_TRIANGLE_STRIP_ADJACENCY, "TriangleStripAdjacency");
	LuaHelper::SetKey<lua_Integer>(State, AbsIndex, GL_TRIANGLES_ADJACENCY, "TrianglesAdjacency");

	LuaHelper::SetKey<lua_Integer>(State, AbsIndex, GL_PATCHES, "Patches");

	lua_setfield(State, AbsIndex - 1, "DrawModes");



	// OpenGL.VertexArray
	VertexArray::Init(State);

	// OpenGL.VertexBuffer
	BufferBase::Init<GL_ARRAY_BUFFER>(State, "VertexBuffer");
	// OpenGL.ElementBuffer
	BufferBase::Init<GL_ELEMENT_ARRAY_BUFFER>(State, "ElementBuffer");

	// OpenGL.Program
	Program::Init(State);
	// OpenGL.Shader
	Shader::Init(State);


	lua_settop(State, StartingStackIndex);
}