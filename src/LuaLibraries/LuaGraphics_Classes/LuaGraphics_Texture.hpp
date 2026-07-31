#pragma once

#include "../../define.h"

#include <SDL3/SDL_opengl.h>
#include <SDL3/SDL_opengl_glext.h>

#include "../../Client/Graphics_Client.hpp"

#include "../../../include/VM/lua.h"
#include "../../../include/VM/lualib.h"
#include "../../FunctionHeaders/LuaHelper.hpp"

//#include "../LuaBuffer.hpp"


#include "../../Statistics.hpp"


namespace Game::Lua::CLibraries::Graphics {

	namespace Classes {
		struct Texture {

			static inline const char* const MetatableName = "Texture";

			GLuint TextureObject;

			static int Bind(lua_State* State);
			
			static int __gc(lua_State* State);
		};
	}

	namespace Texture {

		static inline const char* const LibraryName = "Texture";

		inline void Init(lua_State* State, LuaHelper::StackTableReference& GraphicsTable);

		static int Unbind(lua_State* State);
		static int SetActiveTexture(lua_State* State);

		static int SetFilteringUpscale(lua_State* State);
		static int SetFilteringDownscale(lua_State* State);

		static int SetWrappingHorizontal(lua_State* State);
		static int SetWrappingVertical(lua_State* State);

		static int SetTexImage2D(lua_State* State);

		static int __new(lua_State* State);
	}
}

void Game::Lua::CLibraries::Graphics::Texture::Init(lua_State* State, LuaHelper::StackTableReference& GraphicsTable) {

	LuaHelper::StackTableReference OpenGLShader, ShaderMetatable;


	ShaderMetatable = LuaHelper::StackTableReference(State, CLibraries::Graphics::Classes::Texture::MetatableName);

	ShaderMetatable.SetKeyClosure(State, CLibraries::Graphics::Classes::Texture::Bind, "__gc");

	ShaderMetatable.SetKeyClosure(State, CLibraries::Graphics::Classes::Texture::__gc, "__gc");

	ShaderMetatable.PushReference(State);
	lua_setfield(State, ShaderMetatable.GetStackIndex(), "__index");

	lua_settop(State, ShaderMetatable.GetStackIndex() - 1);



	OpenGLShader = LuaHelper::StackTableReference(State, 0, 1);

	OpenGLShader.SetKeyClosure(State, CLibraries::Graphics::Texture::__new, "new");

	lua_setfield(State, GraphicsTable.GetStackIndex(), CLibraries::Graphics::Texture::LibraryName);
}


int Game::Lua::CLibraries::Graphics::Texture::Unbind(lua_State* State) {
	Game::Graphics::OpenGLFunctions::glBindTexture(static_cast<GLenum>(luaL_checkinteger(State, 1)), 0);
	return 0;
}


int Game::Lua::CLibraries::Graphics::Texture::SetActiveTexture(lua_State* State) {
	Game::Graphics::OpenGLFunctions::glActiveTexture(GL_TEXTURE0 + static_cast<GLenum>(luaL_checkinteger(State, 1)));
	return 0;
}


int Game::Lua::CLibraries::Graphics::Texture::SetTexImage2D(lua_State* State) {

	//using BufferClass = CLibraries::Buffer::Classes::Buffer;

	const char* Buffer;
	size_t BufferSize;
	lua_Integer Height, Width;
	Width = luaL_checkinteger(State, 4);
	Height = luaL_checkinteger(State, 5);
	Buffer = static_cast<const char*>(luaL_checkbuffer(State, 9, &BufferSize));

	if (Width * Height > static_cast<lua_Integer>(BufferSize)) {
		luaL_error(State, "insert error msg about width height of texture exceeding buffer size :P");
	}

	Game::Graphics::OpenGLFunctions::glTexImage2D(
		static_cast<GLenum>(luaL_checkinteger(State, 1)),	// Texture Binding
		static_cast<GLint>(luaL_checkinteger(State, 2)),	// Level
		static_cast<GLenum>(luaL_checkinteger(State, 3)),	// Internal Format
		static_cast<GLint>(Width),							// Width
		static_cast<GLint>(Height),							// Height
		static_cast<GLint>(luaL_optinteger(State, 6, 0)),	// Border
		static_cast<GLenum>(luaL_checkinteger(State, 7)),	// Format
		static_cast<GLenum>(luaL_checkinteger(State, 8)),	// Type
		reinterpret_cast<const void*>(Buffer)		// Pixels
	);
	return 0;
}


int Game::Lua::CLibraries::Graphics::Texture::SetFilteringUpscale(lua_State* State) {
	Game::Graphics::OpenGLFunctions::glTexParameteri(
		GL_TEXTURE_2D,
		static_cast<GLenum>(luaL_checkinteger(State, 1)),
		static_cast<GLenum>(luaL_checkinteger(State, 2))
		);
	return 0;
}
int Game::Lua::CLibraries::Graphics::Texture::SetFilteringDownscale(lua_State* State) {
	Game::Graphics::OpenGLFunctions::glTexParameteri(
		GL_TEXTURE_2D,
		static_cast<GLenum>(luaL_checkinteger(State, 1)),
		static_cast<GLenum>(luaL_checkinteger(State, 2))
	);
	return 0;
}

int Game::Lua::CLibraries::Graphics::Texture::SetWrappingHorizontal(lua_State* State);
int Game::Lua::CLibraries::Graphics::Texture::SetWrappingVertical(lua_State* State);


int Game::Lua::CLibraries::Graphics::Texture::__new(lua_State* State) {
	using TextureStruct = CLibraries::Graphics::Classes::Texture;

	TextureStruct* const TextureUD = static_cast<TextureStruct*>(lua_newuserdata(State, sizeof(TextureStruct)));
	Game::Graphics::OpenGLFunctions::glGenTextures(1, &TextureUD->TextureObject);
	luaL_getmetatable(State, TextureStruct::MetatableName);
	lua_setmetatable(State, -2);

	return 1;
}



int Game::Lua::CLibraries::Graphics::Classes::Texture::Bind(lua_State* State) {
	using TextureStruct = CLibraries::Graphics::Classes::Texture;

	Game::Graphics::OpenGLFunctions::glBindTexture(GL_TEXTURE_2D, static_cast<TextureStruct*>(luaL_checkudata(State, 1, TextureStruct::MetatableName))->TextureObject);
	return 0;
}


int Game::Lua::CLibraries::Graphics::Classes::Texture::__gc(lua_State* State) {
	using TextureStruct = CLibraries::Graphics::Classes::Texture;

	Game::Graphics::OpenGLFunctions::glDeleteTextures(1, &static_cast<TextureStruct*>(luaL_checkudata(State, 1, TextureStruct::MetatableName))->TextureObject);
	return 0;
}