
namespace Game::Lua::CLibraries::Graphics {

	namespace Classes {
		struct Shader {
			static inline void InitMetatable(lua_State* State);

			GLint ShaderObject;
			GLenum ShaderType;

			static int Compile(lua_State* State);

			static int __gc(lua_State* State);
			static int __eq(lua_State* State);
		};
	}

	namespace Shader {
		static inline void Init(lua_State* State);

		static int Create(lua_State* State);
	}
}

void Game::Lua::CLibraries::Graphics::Shader::Init(lua_State* State) {

	lua_createtable(State, 0, 1); // OpenGL.Shader

	lua_pushcfunction(State, Shader::Create);
	lua_setfield(State, -2, "Create");

	lua_setfield(State, -2, "Shader");
}

int Game::Lua::CLibraries::Graphics::Shader::Create(lua_State* State) {

	const GLenum ShaderType = static_cast<GLenum>(luaL_checkinteger(State, 1));
	lua_settop(State, 0);

	Classes::Shader* ShaderUD = reinterpret_cast<Classes::Shader*>(lua_newuserdata(State, sizeof(Classes::Shader)));
	ShaderUD->ShaderObject = glCreateShader(ShaderType);
	ShaderUD->ShaderType = ShaderType;

	luaL_setmetatable(State, "Shader");

	return 1;
}


void Game::Lua::CLibraries::Graphics::Classes::Shader::InitMetatable(lua_State* State) {

	luaL_newmetatable(State, "Shader");

	lua_pushcfunction(State, Shader::Compile);
	lua_setfield(State, -2, "Compile");

	lua_pushcfunction(State, Shader::__gc);
	lua_setfield(State, -2, "__gc");

	lua_pushcfunction(State, Shader::__eq);
	lua_setfield(State, -2, "__eq");
}

int Game::Lua::CLibraries::Graphics::Classes::Shader::Compile(lua_State* State) {
	glBindVertexArray(reinterpret_cast<VertexArray*>(lua_touserdata(State, 1))->GLObject);
	return 0;
}

int Game::Lua::CLibraries::Graphics::Classes::Shader::__gc(lua_State* State) {
	glDeleteShader(reinterpret_cast<Shader*>(lua_touserdata(State, 1))->ShaderObject);
	return 0;
}

int Game::Lua::CLibraries::Graphics::Classes::Shader::__eq(lua_State* State) {

	int Result;
	if (const Shader* const B = reinterpret_cast<Shader*>(luaL_checkudata(State, 2, "Shader"))) {
		Result = static_cast<int>(reinterpret_cast<Shader*>(lua_touserdata(State, 1))->ShaderObject == B->ShaderObject);
	} else {
		Result = static_cast<int>(false);
	}

	lua_settop(State, 0);
	lua_pushboolean(State, Result);
	return 1;
}