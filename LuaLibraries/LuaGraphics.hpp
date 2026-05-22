#pragma once

#include <lua-5.5.0/lua.hpp>

#ifndef BUILD_SERVER

#include "LuaGraphics_Classes/LuaGraphics_GLObjectBase.hpp"
#include "LuaGraphics_Classes/LuaGraphics_BufferBase.hpp"

#include "LuaGraphics_Classes/LuaGraphics_VertexArray.hpp"
#include "LuaGraphics_Classes/LuaGraphics_VertexBuffer.hpp"
#include "LuaGraphics_Classes/LuaGraphics_ElementBuffer.hpp"

#include "LuaGraphics_Classes/LuaGraphics_UniformBuffer.hpp"

#include "LuaGraphics_Classes/LuaGraphics_Shader.hpp"
#include "LuaGraphics_Classes/LuaGraphics_Program.hpp"

#endif

#include "../ConsoleVars.hpp"

namespace Game::ConsoleVars::ReferenceVars::Lua::Graphics {

	struct ChangeErrorCheckingCVar : Game::ConsoleVars::BaseCVar {

		virtual void Set(const char* Argument) override {
			
		}

		using VarType = void;
	};
}


namespace Game::Lua::CLibraries::Graphics {
	inline void Init(lua_State* State);

#ifndef BUILD_SERVER
	static int DrawArrays(lua_State* State);
	static int DrawElements(lua_State* State);

	static int DrawArraysInstanced(lua_State* State);
	static int DrawElementsInstanced(lua_State* State);
#endif
}

#ifndef BUILD_SERVER
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
#endif




void Game::Lua::CLibraries::Graphics::Init(lua_State* State) {

	LuaHelper::StackTableReference GraphicsTable, SubtableCache;

	// Graphics

	GraphicsTable = LuaHelper::StackTableReference(State, 0, 16);

#ifndef BUILD_SERVER
	GraphicsTable.SetKeyClosure(State, Graphics::DrawArrays, "DrawArrays");
	GraphicsTable.SetKeyClosure(State, Graphics::DrawElements, "DrawElements");

	GraphicsTable.SetKeyClosure(State, Graphics::DrawArraysInstanced, "DrawArraysInstanced");
	GraphicsTable.SetKeyClosure(State, Graphics::DrawElementsInstanced, "DrawElementsInstanced");
#endif

	// DrawModes
	SubtableCache = LuaHelper::StackTableReference(State, 0, 12);

	SubtableCache.SetKey<lua_Integer>(State, GL_POINTS, "Points");

	SubtableCache.SetKey<lua_Integer>(State, GL_LINE_STRIP, "LineStrip");
	SubtableCache.SetKey<lua_Integer>(State, GL_LINE_LOOP, "LineLoop");
	SubtableCache.SetKey<lua_Integer>(State, GL_LINES, "Lines");
	SubtableCache.SetKey<lua_Integer>(State, GL_LINE_STRIP_ADJACENCY, "LineStripAdjacency");
	SubtableCache.SetKey<lua_Integer>(State, GL_LINES_ADJACENCY, "LinesAdjacency");

	SubtableCache.SetKey<lua_Integer>(State, GL_TRIANGLE_STRIP, "TriangleStrip");
	SubtableCache.SetKey<lua_Integer>(State, GL_TRIANGLE_FAN, "TriangleFan");
	SubtableCache.SetKey<lua_Integer>(State, GL_TRIANGLES, "Triangles");
	SubtableCache.SetKey<lua_Integer>(State, GL_TRIANGLE_STRIP_ADJACENCY, "TriangleStripAdjacency");
	SubtableCache.SetKey<lua_Integer>(State, GL_TRIANGLES_ADJACENCY, "TrianglesAdjacency");

	SubtableCache.SetKey<lua_Integer>(State, GL_PATCHES, "Patches");

	lua_setfield(State, GraphicsTable.GetStackIndex(), "DrawModes");


	// ShaderTypes
	SubtableCache = LuaHelper::StackTableReference(State, 0, 6);

	SubtableCache.SetKey<lua_Integer>(State, GL_COMPUTE_SHADER, "ComputeShader");

	SubtableCache.SetKey<lua_Integer>(State, GL_VERTEX_SHADER, "VertexShader");

	SubtableCache.SetKey<lua_Integer>(State, GL_TESS_CONTROL_SHADER, "TessControlShader");
	SubtableCache.SetKey<lua_Integer>(State, GL_TESS_EVALUATION_SHADER, "TessEvalShader");

	SubtableCache.SetKey<lua_Integer>(State, GL_GEOMETRY_SHADER, "GeometryShader");
	SubtableCache.SetKey<lua_Integer>(State, GL_FRAGMENT_SHADER, "FragmentShader");

	lua_setfield(State, GraphicsTable.GetStackIndex(), "ShaderTypes");


	// Types
	SubtableCache = LuaHelper::StackTableReference(State, 0, 10);

	SubtableCache.SetKey<lua_Integer>(State, GL_UNSIGNED_BYTE, "Uint8");
	SubtableCache.SetKey<lua_Integer>(State, GL_UNSIGNED_SHORT, "Uint16");
	SubtableCache.SetKey<lua_Integer>(State, GL_UNSIGNED_INT, "Uint32");

	SubtableCache.SetKey<lua_Integer>(State, GL_BYTE, "Int8");
	SubtableCache.SetKey<lua_Integer>(State, GL_SHORT, "Int16");
	SubtableCache.SetKey<lua_Integer>(State, GL_INT, "Int32");

	SubtableCache.SetKey<lua_Integer>(State, GL_FLOAT, "F32");
	SubtableCache.SetKey<lua_Integer>(State, GL_FLOAT, "Float");

	SubtableCache.SetKey<lua_Integer>(State, GL_DOUBLE, "F64");
	SubtableCache.SetKey<lua_Integer>(State, GL_DOUBLE, "Double");

	lua_setfield(State, GraphicsTable.GetStackIndex(), "Types");


#ifndef BUILD_SERVER
	Classes::VertexBuffer::InitMetatable(State, "VertexBuffer");
	Classes::ElementBuffer::InitMetatable(State, "ElementBuffer");

	Classes::UniformBuffer::InitMetatable(State);


	VertexArray::Init(State, GraphicsTable);
	VertexBuffer::Init<GL_ARRAY_BUFFER>(State, "VertexBuffer", GraphicsTable);
	ElementBuffer::Init<GL_ELEMENT_ARRAY_BUFFER>(State, "ElementBuffer", GraphicsTable);

	UniformBuffer::Init<GL_UNIFORM_BUFFER>(State, "UniformBuffer", GraphicsTable);


	Program::Init(State, GraphicsTable);
	Shader::Init(State, GraphicsTable);

#endif

	lua_settop(State, GraphicsTable.GetStackIndex());
	lua_setfield(State, Game::Lua::GameTable.GetStackIndex(), "Graphics");
}