
namespace Game::Lua::CLibraries::Graphics {

	namespace Classes {
		struct Program : Game::Lua::CLibraries::Graphics::Classes::GLObjectBase {
			static inline void InitMetatable(lua_State* State);

			static int AttachShader(lua_State* State);
			static int DetachShader(lua_State* State);

			static int Link(lua_State* State);
			static int Use(lua_State* State);

			static int __gc(lua_State* State);
			static int __eq(lua_State* State);
		};
	}

	namespace Program {
		static inline void Init(lua_State* State);

		static int Create(lua_State* State);

		static int SetUniformInt(lua_State* State);
		static int SetUniformUint(lua_State* State);
		static int SetUniformFloat(lua_State* State);

		static int SetUniformIvec2(lua_State* State);
		static int SetUniformUvec2(lua_State* State);
		static int SetUniformBvec2(lua_State* State);
		static int SetUniformFvec2(lua_State* State);

		static int SetUniformIvec3(lua_State* State);
		static int SetUniformUvec3(lua_State* State);
		static int SetUniformBvec3(lua_State* State);
		static int SetUniformFvec3(lua_State* State);

		static int SetUniformIvec4(lua_State* State);
		static int SetUniformUvec4(lua_State* State);
		static int SetUniformBvec4(lua_State* State);
		static int SetUniformFvec4(lua_State* State);
	}
}

void Game::Lua::CLibraries::Graphics::Program::Init(lua_State* State) {

	LuaHelper::StackTableReference ProgramTable(State, 0, 17); // OpenGL.Program

	ProgramTable.SetKey(State, Program::Create, "Create");

	ProgramTable.SetKey(State, Program::SetUniformInt, "SetUniformInt");
	ProgramTable.SetKey(State, Program::SetUniformUint, "SetUniformUint");
	ProgramTable.SetKey(State, Program::SetUniformInt, "SetUniformBool");
	ProgramTable.SetKey(State, Program::SetUniformFloat, "SetUniformFloat");

	ProgramTable.SetKey(State, Program::SetUniformIvec2, "SetUniformIvec2");
	ProgramTable.SetKey(State, Program::SetUniformUvec2, "SetUniformUvec2");
	ProgramTable.SetKey(State, Program::SetUniformBvec2, "SetUniformBvec2");
	ProgramTable.SetKey(State, Program::SetUniformFvec2, "SetUniformVec2");

	ProgramTable.SetKey(State, Program::SetUniformIvec3, "SetUniformIvec3");
	ProgramTable.SetKey(State, Program::SetUniformUvec3, "SetUniformUvec3");
	ProgramTable.SetKey(State, Program::SetUniformBvec3, "SetUniformBvec3");
	ProgramTable.SetKey(State, Program::SetUniformFvec3, "SetUniformVec3");

	ProgramTable.SetKey(State, Program::SetUniformIvec4, "SetUniformIvec4");
	ProgramTable.SetKey(State, Program::SetUniformUvec4, "SetUniformUvec4");
	ProgramTable.SetKey(State, Program::SetUniformBvec4, "SetUniformBvec4");
	ProgramTable.SetKey(State, Program::SetUniformFvec4, "SetUniformVec4");

	lua_setfield(State, ProgramTable.GetStackIndex() - 1, "Program");
}

int Game::Lua::CLibraries::Graphics::Program::Create(lua_State* State) {

	Classes::Program* ProgramUD = static_cast<Classes::Program*>(lua_newuserdata(State, sizeof(Classes::Program)));
	ProgramUD->GLObject = glCreateProgram();

	luaL_setmetatable(State, "Program");

	return 1;
}



namespace Game::Lua::CLibraries::Graphics::Program {

	int SetUniformInt(lua_State* State) {
		glUniform1i(
			static_cast<GLint>(luaL_checkinteger(State, 1)),
			static_cast<GLint>(luaL_checkinteger(State, 2))
		);
		return 0;
	}

	int SetUniformUint(lua_State* State) {
		glUniform1ui(
			static_cast<GLint>(luaL_checkinteger(State, 1)),
			static_cast<GLuint>(luaL_checkinteger(State, 2))
		);
		return 0;
	}

	int SetUniformFloat(lua_State* State) {
		glUniform1f(
			static_cast<GLint>(luaL_checkinteger(State, 1)),
			static_cast<GLfloat>(luaL_checknumber(State, 2))
		);
		return 0;
	}
}

namespace Game::Lua::CLibraries::Graphics::Program {

	int SetUniformIvec2(lua_State* State) {

		if (const void* VecUD = luaL_testudata(State, 2, "IVector2"); VecUD == NULL) {
			glUniform2i(
				static_cast<GLint>(luaL_checkinteger(State, 1)),
				static_cast<GLint>(luaL_checkinteger(State, 2)),
				static_cast<GLint>(luaL_checkinteger(State, 3))
			);
		} else {
			glUniform1iv(
				static_cast<GLint>(luaL_checkinteger(State, 1)),
				2,
				static_cast<const GLint*>(VecUD)
			);
		}

		return 0;
	}

	int SetUniformUvec2(lua_State* State) {
		if (const void* VecUD = luaL_testudata(State, 2, "UVector2"); VecUD == NULL) {
			glUniform2ui(
				static_cast<GLint>(luaL_checkinteger(State, 1)),
				static_cast<GLuint>(luaL_checkinteger(State, 2)),
				static_cast<GLuint>(luaL_checkinteger(State, 3))
			);
		} else {
			glUniform1uiv(
				static_cast<GLint>(luaL_checkinteger(State, 1)),
				2,
				static_cast<const GLuint*>(VecUD)
			);
		}

		return 0;
	}

	int SetUniformBvec2(lua_State* State) {
		glUniform2i(
			static_cast<GLint>(luaL_checkinteger(State, 1)),
			static_cast<GLint>(luaL_checkinteger(State, 2)),
			static_cast<GLint>(luaL_checkinteger(State, 3))
		);

		return 0;
	}

	int SetUniformFvec2(lua_State* State) {

		if (const void* VecUD = luaL_testudata(State, 2, "Vector2"); VecUD == NULL) {
			glUniform2f(
				static_cast<GLint>(luaL_checkinteger(State, 1)),
				static_cast<GLfloat>(luaL_checknumber(State, 2)),
				static_cast<GLfloat>(luaL_checknumber(State, 3))
			);
		} else {
			glUniform1fv(
				static_cast<GLint>(luaL_checkinteger(State, 1)),
				2,
				static_cast<const GLfloat*>(VecUD)
			);
		}

		return 0;
	}
}

namespace Game::Lua::CLibraries::Graphics::Program {

	int SetUniformIvec3(lua_State* State) {

		if (const void* VecUD = luaL_testudata(State, 2, "IVector3"); VecUD == NULL) {
			glUniform3i(
				static_cast<GLint>(luaL_checkinteger(State, 1)),
				static_cast<GLint>(luaL_checkinteger(State, 2)),
				static_cast<GLint>(luaL_checkinteger(State, 3)),
				static_cast<GLint>(luaL_checkinteger(State, 4))
			);
		} else {
			glUniform1iv(
				static_cast<GLint>(luaL_checkinteger(State, 1)),
				3,
				static_cast<const GLint*>(VecUD)
			);
		}
		return 0;
	}

	int SetUniformUvec3(lua_State* State) {

		if (const void* VecUD = luaL_testudata(State, 2, "UVector3"); VecUD == NULL) {
			glUniform3ui(
				static_cast<GLint>(luaL_checkinteger(State, 1)),
				static_cast<GLuint>(luaL_checkinteger(State, 2)),
				static_cast<GLuint>(luaL_checkinteger(State, 3)),
				static_cast<GLuint>(luaL_checkinteger(State, 4))
			);
		} else {
			glUniform1uiv(
				static_cast<GLint>(luaL_checkinteger(State, 1)),
				3,
				static_cast<const GLuint*>(VecUD)
			);
		}
		return 0;
	}

	int SetUniformBvec3(lua_State* State) {
		glUniform3i(
			static_cast<GLint>(luaL_checkinteger(State, 1)),
			static_cast<GLint>(luaL_checkinteger(State, 2)),
			static_cast<GLint>(luaL_checkinteger(State, 3)),
			static_cast<GLint>(luaL_checkinteger(State, 4))
		);
		return 0;
	}

	int SetUniformFvec3(lua_State* State) {

		if (const void* VecUD = luaL_testudata(State, 2, "Vector3"); VecUD == NULL) {
			glUniform3f(
				static_cast<GLint>(luaL_checkinteger(State, 1)),
				static_cast<GLfloat>(luaL_checknumber(State, 2)),
				static_cast<GLfloat>(luaL_checknumber(State, 3)),
				static_cast<GLfloat>(luaL_checknumber(State, 4))
			);
		} else {
			glUniform1fv(
				static_cast<GLint>(luaL_checkinteger(State, 1)),
				3,
				static_cast<const GLfloat*>(VecUD)
			);
		}
		return 0;
	}
}

namespace Game::Lua::CLibraries::Graphics::Program {

	int SetUniformIvec4(lua_State* State) {

		if (const void* VecUD = luaL_testudata(State, 2, "IVector4"); VecUD == NULL) {
			glUniform4i(
				static_cast<GLint>(luaL_checkinteger(State, 1)),
				static_cast<GLint>(luaL_checkinteger(State, 2)),
				static_cast<GLint>(luaL_checkinteger(State, 3)),
				static_cast<GLint>(luaL_checkinteger(State, 4)),
				static_cast<GLint>(luaL_checkinteger(State, 5))
			);
		} else {
			glUniform1iv(
				static_cast<GLint>(luaL_checkinteger(State, 1)),
				4,
				static_cast<const GLint*>(VecUD)
			);
		}
		return 0;
	}

	int SetUniformUvec4(lua_State* State) {

		if (const void* VecUD = luaL_testudata(State, 2, "UVector4"); VecUD == NULL) {
			glUniform4ui(
				static_cast<GLint>(luaL_checkinteger(State, 1)),
				static_cast<GLuint>(luaL_checkinteger(State, 2)),
				static_cast<GLuint>(luaL_checkinteger(State, 3)),
				static_cast<GLuint>(luaL_checkinteger(State, 4)),
				static_cast<GLuint>(luaL_checkinteger(State, 5))
			);
		} else {
			glUniform1uiv(
				static_cast<GLint>(luaL_checkinteger(State, 1)),
				4,
				static_cast<const GLuint*>(VecUD)
			);
		}
		return 0;
	}

	int SetUniformBvec4(lua_State* State) {
		glUniform4i(
			static_cast<GLint>(luaL_checkinteger(State, 1)),
			static_cast<GLint>(luaL_checkinteger(State, 2)),
			static_cast<GLint>(luaL_checkinteger(State, 3)),
			static_cast<GLint>(luaL_checkinteger(State, 4)),
			static_cast<GLint>(luaL_checkinteger(State, 5))
		);
		return 0;
	}

	int SetUniformFvec4(lua_State* State) {

		if (const void* VecUD = luaL_testudata(State, 2, "Vector4"); VecUD == NULL) {
			glUniform4f(
				static_cast<GLint>(luaL_checkinteger(State, 1)),
				static_cast<GLfloat>(luaL_checknumber(State, 2)),
				static_cast<GLfloat>(luaL_checknumber(State, 3)),
				static_cast<GLfloat>(luaL_checknumber(State, 4)),
				static_cast<GLfloat>(luaL_checknumber(State, 5))
			);
		} else {
			glUniform1fv(
				static_cast<GLint>(luaL_checkinteger(State, 1)),
				4,
				static_cast<const GLfloat*>(VecUD)
			);
		}
		return 0;
	}
}





void Game::Lua::CLibraries::Graphics::Classes::Program::InitMetatable(lua_State* State) {

	LuaHelper::StackTableReference ProgramMetatable(State, "Program");

	ProgramMetatable.SetKey(State, Program::AttachShader, "AttachShader");
	ProgramMetatable.SetKey(State, Program::DetachShader, "DetachShader");

	ProgramMetatable.SetKey(State, Program::Link, "Link");
	ProgramMetatable.SetKey(State, Program::Use, "Use");

	ProgramMetatable.SetKey(State, Program::__eq, "__eq");
	ProgramMetatable.SetKey(State, Program::__gc, "__gc");
}

int Game::Lua::CLibraries::Graphics::Classes::Program::AttachShader(lua_State* State) {

	const Shader* const ShaderUD = static_cast<Shader*>(luaL_testudata(State, 2, "Shader"));
	unlikely_branch
		if (ShaderUD == NULL) {
			luaL_error(State, "Attempted to attach a non-shader object to a OpenGL program.");
		}

	glAttachShader(static_cast<Program*>(lua_touserdata(State, 1))->GLObject, ShaderUD->ShaderObject);
	return 0;
}

int Game::Lua::CLibraries::Graphics::Classes::Program::DetachShader(lua_State* State) {

	const Shader* const ShaderUD = static_cast<Shader*>(luaL_testudata(State, 2, "Shader"));
	unlikely_branch
		if (ShaderUD == NULL) {
			luaL_error(State, "Attempted to detatch a non-shader object from a OpenGL program.");
		}

	glDetachShader(static_cast<Program*>(lua_touserdata(State, 1))->GLObject, ShaderUD->ShaderObject);
	return 0;
}

int Game::Lua::CLibraries::Graphics::Classes::Program::Link(lua_State* State) {

	Program* const ProgramUD = static_cast<Program*>(lua_touserdata(State, 1));
	glLinkProgram(ProgramUD->GLObject);
	return 0;
}

int Game::Lua::CLibraries::Graphics::Classes::Program::Use(lua_State* State) {
	glUseProgram(static_cast<Program*>(lua_touserdata(State, 1))->GLObject);
	return 0;
}

int Game::Lua::CLibraries::Graphics::Classes::Program::__gc(lua_State* State) {
	glDeleteProgram(static_cast<Program*>(lua_touserdata(State, 1))->GLObject);
	return 0;
}

int Game::Lua::CLibraries::Graphics::Classes::Program::__eq(lua_State* State) {
	int Result;
	if (const Program* const B = static_cast<Program*>(luaL_testudata(State, 2, "Program"))) {
		Result = static_cast<int>(static_cast<Program*>(lua_touserdata(State, 1))->GLObject == B->GLObject);
	} else {
		Result = static_cast<int>(false);
	}

	lua_settop(State, 0);
	lua_pushboolean(State, Result);
	return 1;
}