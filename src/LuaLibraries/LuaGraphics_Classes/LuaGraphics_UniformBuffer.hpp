#pragma once

#include "LuaGraphics_BufferBase.hpp"


namespace Game::Lua::CLibraries::Graphics::Classes {
	struct UniformBuffer : Game::Lua::CLibraries::Graphics::Classes::BufferBase {

		static inline void InitMetatable(lua_State* State) {

			LuaHelper::StackTableReference UniformBufferMetatable = BufferBase::InitMetatable(State, "UniformBuffer");

			UniformBufferMetatable.PushReference(State);
			UniformBufferMetatable.SetKeyClosure(State, UniformBuffer::BindToUniformBlockIndex, "BindToUniformBlockIndex", 1);

			UniformBufferMetatable.PushReference(State);
			UniformBufferMetatable.SetKeyClosure(State, UniformBuffer::BindToUniformBlockIndexRange, "BindToUniformBlockIndexRange", 1);

			lua_settop(State, UniformBufferMetatable.GetStackIndex() - 1);
		}

		static int BindToUniformBlockIndex(lua_State* State) {
			Game::Graphics::OpenGLFunctions::glBindBufferBase(
				GL_UNIFORM_BUFFER,
				static_cast<GLuint>(luaL_checkinteger(State, 2)),	// UniformBlockIndex
				static_cast<UniformBuffer*>(luaL_checkudata(State, 1, "UniformBuffer"))->GLObject
			);
			return 0;
		}

		static int BindToUniformBlockIndexRange(lua_State* State) {
			Game::Graphics::OpenGLFunctions::glBindBufferRange(
				GL_UNIFORM_BUFFER,
				static_cast<GLuint>(luaL_checkinteger(State, 2)),		// UniformBlockIndex
				static_cast<UniformBuffer*>(luaL_checkudata(State, 1, "UniformBuffer"))->GLObject,
				static_cast<GLintptr>(luaL_checkinteger(State, 3)),		// Offset
				static_cast<GLsizeiptr>(luaL_checkinteger(State, 4))	// Size
			);
			return 0;
		}
	};
}

namespace Game::Lua::CLibraries::Graphics::UniformBuffer {
	using namespace Game::Lua::CLibraries::Graphics::BufferBase;
}