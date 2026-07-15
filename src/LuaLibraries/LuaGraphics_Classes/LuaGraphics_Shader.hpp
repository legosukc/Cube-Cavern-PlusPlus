#pragma once

#include <SDL3/SDL_opengl.h>
#include <SDL3/SDL_opengl_glext.h>

#include <SDL3/SDL_timer.h>


#include <lua.hpp>
#include "../../FunctionHeaders/LuaHelper.hpp"

#include "../../Statistics.hpp"


namespace Game::Lua::CLibraries::Graphics {

	namespace Classes {
		struct Shader {

			GLint ShaderObject;
			GLenum ShaderType;

			static int SetShaderSource(lua_State* State);
			static int LoadShaderSourceFromFile(lua_State* State);

			static int GetShaderSource(lua_State* State);

			static int Compile(lua_State* State);

			static int __gc(lua_State* State);
		};
	}

	namespace Shader {
		static inline void Init(lua_State* State, LuaHelper::StackTableReference& GraphicsTable);

		static int __new(lua_State* State);
	}
}

void Game::Lua::CLibraries::Graphics::Shader::Init(lua_State* State, LuaHelper::StackTableReference& GraphicsTable) {

	LuaHelper::StackTableReference OpenGLShader, ShaderMetatable;


	ShaderMetatable = LuaHelper::StackTableReference(State, "Shader");
	
	ShaderMetatable.SetKeyClosure(State, Classes::Shader::SetShaderSource, "SetShaderSource");
	ShaderMetatable.SetKeyClosure(State, Classes::Shader::LoadShaderSourceFromFile, "LoadShaderSourceFromFile");

	ShaderMetatable.SetKeyClosure(State, Classes::Shader::GetShaderSource, "GetShaderSource");

	ShaderMetatable.SetKeyClosure(State, Classes::Shader::Compile, "Compile");

	ShaderMetatable.SetKeyClosure(State, Classes::Shader::__gc, "__gc");
	//ShaderMetatable.SetKeyClosure(State, Classes::Shader::__eq, "__eq");

	ShaderMetatable.PushReference(State);
	lua_setfield(State, ShaderMetatable.GetStackIndex(), "__index");

	lua_settop(State, ShaderMetatable.GetStackIndex() - 1);


	OpenGLShader = LuaHelper::StackTableReference(State, 0, 1);

	OpenGLShader.SetKeyClosure(State, Shader::__new, "new");

	lua_setfield(State, GraphicsTable.GetStackIndex(), "Shader");
}

int Game::Lua::CLibraries::Graphics::Shader::__new(lua_State* State) {

	lua_settop(State, 1);

	Classes::Shader* const ShaderUD = static_cast<Classes::Shader*>(lua_newuserdata(State, sizeof(Classes::Shader)));
	ShaderUD->ShaderType = static_cast<GLenum>(luaL_checkinteger(State, 1));
	ShaderUD->ShaderObject = Game::Graphics::OpenGLFunctions::glCreateShader(ShaderUD->ShaderType);

	luaL_setmetatable(State, "Shader");

	return 1;
}



int Game::Lua::CLibraries::Graphics::Classes::Shader::SetShaderSource(lua_State* State) {

	const char** ShaderSources;
	int StackTop;
	GLuint ShaderObject;

	ShaderObject = static_cast<Classes::Shader*>(luaL_checkudata(State, 1, "Shader"))->ShaderObject;


	StackTop = lua_gettop(State);
	ShaderSources = new const char*[StackTop - 1];
	for (int i = 2; i <= StackTop; ++i) {
		ShaderSources[i - 2] = lua_tostring(State, i);
	}

	Game::Graphics::OpenGLFunctions::glShaderSource(ShaderObject, StackTop - 1, ShaderSources, NULL);
	delete[] ShaderSources;

	return 0;
}

int Game::Lua::CLibraries::Graphics::Classes::Shader::LoadShaderSourceFromFile(lua_State* State) {

	lua_warning(State, "not implemented!", true);

	return 0;
}

int Game::Lua::CLibraries::Graphics::Classes::Shader::GetShaderSource(lua_State* State) {

	char* ShaderSource;
	GLint ShaderSourceLength;
	GLuint ShaderObject;
	Uint64 StartNS;

	StartNS = SDL_GetTicksNS();

	ShaderObject = static_cast<Classes::Shader*>(luaL_checkudata(State, 1, "Shader"))->ShaderObject;

	Game::Graphics::OpenGLFunctions::glGetShaderiv(ShaderObject, GL_SHADER_SOURCE_LENGTH, &ShaderSourceLength);

	// directly call std::malloc to not update Game::Statistics::Memory stats.
	ShaderSource = static_cast<char*>(std::malloc(ShaderSourceLength));

	++Game::Statistics::Memory::EngineAllocationsPerFrame;
	Game::Statistics::Memory::EngineAllocationBytesPerFrame += ShaderSourceLength;

	Game::Graphics::OpenGLFunctions::glGetShaderSource(ShaderObject, ShaderSourceLength, NULL, ShaderSource);
	
	LuaHelper::PushExternalString(State, ShaderSource, ShaderSourceLength);

	//++Game::Statistics::Memory::EngineDeallocationsPerFrame;
	Game::Statistics::Memory::EngineNSSpentOnHeapPerFrame += SDL_GetTicksNS() - StartNS;

	return 1;
}


int Game::Lua::CLibraries::Graphics::Classes::Shader::Compile(lua_State* State) {

	char* InfoLog;
	GLint InfoLogSize, Success;
	GLuint ShaderObject;

	ShaderObject = static_cast<Shader*>(luaL_checkudata(State, 1, "Shader"))->ShaderObject;
	//lua_settop(State, 0);

	Game::Graphics::OpenGLFunctions::glCompileShader(ShaderObject);

	Game::Graphics::OpenGLFunctions::glGetShaderiv(ShaderObject, GL_COMPILE_STATUS, &Success);
	if (!Success) {

		Game::Graphics::OpenGLFunctions::glGetShaderiv(ShaderObject, GL_INFO_LOG_LENGTH, &InfoLogSize);

		InfoLog = static_cast<char*>(std::malloc(InfoLogSize));
		++Game::Statistics::Memory::EngineAllocationsPerFrame;

		Game::Graphics::OpenGLFunctions::glGetShaderInfoLog(ShaderObject, InfoLogSize, NULL, InfoLog);

		LuaHelper::PushExternalString(State, InfoLog, InfoLogSize);
		return 1;
	}

	return 0;
}

int Game::Lua::CLibraries::Graphics::Classes::Shader::__gc(lua_State* State) {
	Game::Graphics::OpenGLFunctions::glDeleteShader(static_cast<Shader*>(luaL_checkudata(State, 1, "Shader"))->ShaderObject);
	return 0;
}