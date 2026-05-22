#pragma once

#include "LuaGraphics_GLObjectBase.hpp"


#include <lua-5.5.0/lua.hpp>



namespace Game::Lua::CLibraries::Graphics {

	namespace Classes {
		struct Program : Game::Lua::CLibraries::Graphics::Classes::GLObjectBase {
			static int AttachShader(lua_State* State);
			static int DetachShader(lua_State* State);

			static int Link(lua_State* State);
			static int Use(lua_State* State);

			static int GetUniformIndex(lua_State* State);

			static int SetUniformBlockBinding(lua_State* State);
			static int GetUniformBlockIndex(lua_State* State);

			static int __gc(lua_State* State);
			//static int __eq(lua_State* State);
		};
	}

	namespace Program {
		static inline void Init(lua_State* State, LuaHelper::StackTableReference& GraphicsTable);

		static int __new(lua_State* State);

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

		static int SetUniformMat2(lua_State* State);
		static int SetUniformMat3(lua_State* State);
		static int SetUniformMat4(lua_State* State);
	}
}

void Game::Lua::CLibraries::Graphics::Program::Init(lua_State* State, LuaHelper::StackTableReference& GraphicsTable) {

	LuaHelper::StackTableReference ProgramMetatable(State, "Program");

	ProgramMetatable.SetKeyClosure(State, Classes::Program::__gc, "__gc");

	ProgramMetatable.PushReference(State);
	lua_setfield(State, ProgramMetatable.GetStackIndex(), "__index");

	ProgramMetatable.SetKeyClosure(State, Classes::Program::SetUniformBlockBinding, "SetUniformBlockBinding");
	ProgramMetatable.SetKeyClosure(State, Classes::Program::GetUniformBlockIndex, "GetUniformBlockIndex");

	ProgramMetatable.SetKeyClosure(State, Classes::Program::AttachShader, "AttachShader");
	ProgramMetatable.SetKeyClosure(State, Classes::Program::DetachShader, "DetachShader");

	ProgramMetatable.SetKeyClosure(State, Classes::Program::Link, "Link");

	ProgramMetatable.PushReference(State);
	ProgramMetatable.SetKeyClosure(State, Classes::Program::Use, "Use", 1);

	lua_settop(State, ProgramMetatable.GetStackIndex() - 1);



	LuaHelper::StackTableReference ProgramTable(State, 0, 17); // OpenGL.Program

	ProgramTable.SetKeyClosure(State, Program::__new, "new");

	ProgramTable.SetKeyClosure(State, Program::SetUniformInt, "SetUniformInt");
	ProgramTable.SetKeyClosure(State, Program::SetUniformUint, "SetUniformUint");
	ProgramTable.SetKeyClosure(State, Program::SetUniformInt, "SetUniformBool");
	ProgramTable.SetKeyClosure(State, Program::SetUniformFloat, "SetUniformFloat");

	ProgramTable.SetKeyClosure(State, Program::SetUniformIvec2, "SetUniformIvec2");
	ProgramTable.SetKeyClosure(State, Program::SetUniformUvec2, "SetUniformUvec2");
	ProgramTable.SetKeyClosure(State, Program::SetUniformBvec2, "SetUniformBvec2");
	ProgramTable.SetKeyClosure(State, Program::SetUniformFvec2, "SetUniformVec2");

	ProgramTable.SetKeyClosure(State, Program::SetUniformIvec3, "SetUniformIvec3");
	ProgramTable.SetKeyClosure(State, Program::SetUniformUvec3, "SetUniformUvec3");
	ProgramTable.SetKeyClosure(State, Program::SetUniformBvec3, "SetUniformBvec3");
	ProgramTable.SetKeyClosure(State, Program::SetUniformFvec3, "SetUniformVec3");

	ProgramTable.SetKeyClosure(State, Program::SetUniformIvec4, "SetUniformIvec4");
	ProgramTable.SetKeyClosure(State, Program::SetUniformUvec4, "SetUniformUvec4");
	ProgramTable.SetKeyClosure(State, Program::SetUniformBvec4, "SetUniformBvec4");
	ProgramTable.SetKeyClosure(State, Program::SetUniformFvec4, "SetUniformVec4");

	ProgramTable.SetKeyClosure(State, Program::SetUniformMat2, "SetUniformMat2");
	ProgramTable.SetKeyClosure(State, Program::SetUniformMat3, "SetUniformMat3");
	ProgramTable.SetKeyClosure(State, Program::SetUniformMat4, "SetUniformMat4");

	lua_setfield(State, GraphicsTable.GetStackIndex(), "Program");
}

int Game::Lua::CLibraries::Graphics::Program::__new(lua_State* State) {

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

namespace Game::Lua::CLibraries::Graphics::Program {

	int SetUniformMat2(lua_State* State) {

		GLfloat MatArray[2 * 2];
		const void* MatUD;

		if (MatUD = luaL_testudata(State, 3, "Mat2"); MatUD == NULL) {

			for (int i = 3; i <= 2 + (2 * 2); ++i) {
				MatArray[i - 3] = static_cast<GLfloat>(luaL_optnumber(State, i, 0.0));
			}
			MatUD = MatArray;
		}

		glUniformMatrix2fv(static_cast<GLint>(luaL_checkinteger(State, 1)), 1, static_cast<GLboolean>(lua_toboolean(State, 2)), static_cast<const GLfloat*>(MatUD));
		return 0;
	}

	int SetUniformMat3(lua_State* State) {

		GLfloat MatArray[3 * 3];
		const void* MatUD;

		if (MatUD = luaL_testudata(State, 3, "Mat3"); MatUD == NULL) {

			for (int i = 3; i <= 2 + (3 * 3); ++i) {
				MatArray[i - 3] = static_cast<GLfloat>(luaL_optnumber(State, i, 0.0));
			}
			MatUD = MatArray;
		}

		glUniformMatrix3fv(static_cast<GLint>(luaL_checkinteger(State, 1)), 1, static_cast<GLboolean>(lua_toboolean(State, 2)), static_cast<const GLfloat*>(MatUD));
		return 0;
	}

	int SetUniformMat4(lua_State* State) {

		GLfloat MatArray[4 * 4];
		const void* MatUD;

		if (MatUD = luaL_testudata(State, 3, "Mat4"); MatUD == NULL) {

			for (int i = 3; i <= 2 + (4 * 4); ++i) {
				MatArray[i - 3] = static_cast<GLfloat>(luaL_optnumber(State, i, 0.0));
			}
			MatUD = MatArray;
		}

		glUniformMatrix4fv(static_cast<GLint>(luaL_checkinteger(State, 1)), 1, static_cast<GLboolean>(lua_toboolean(State, 2)), static_cast<const GLfloat*>(MatUD));
		return 0;
	}
}






int Game::Lua::CLibraries::Graphics::Classes::Program::AttachShader(lua_State* State) {

	const Classes::Shader* ShaderUD = static_cast<Classes::Shader*>(luaL_testudata(State, 2, "Shader"));
	if (ShaderUD == NULL) {
		luaL_error(State, "Attempted to attach a non-shader object to a OpenGL program.");
	}

	glAttachShader(static_cast<Classes::Program*>(luaL_checkudata(State, 1, "Program"))->GLObject, ShaderUD->ShaderObject);
	return 0;
}

int Game::Lua::CLibraries::Graphics::Classes::Program::DetachShader(lua_State* State) {

	const Classes::Shader* ShaderUD = static_cast<Classes::Shader*>(luaL_testudata(State, 2, "Shader"));
	if (ShaderUD == NULL) {
		luaL_error(State, "Attempted to detatch a non-shader object from a OpenGL program.");
	}

	glDetachShader(static_cast<Classes::Program*>(luaL_checkudata(State, 1, "Program"))->GLObject, ShaderUD->ShaderObject);
	return 0;
}

int Game::Lua::CLibraries::Graphics::Classes::Program::Link(lua_State* State) {

	const Classes::Program* ProgramUD = static_cast<Classes::Program*>(luaL_checkudata(State, 1, "Program"));
	glLinkProgram(ProgramUD->GLObject);
	return 0;
}

int Game::Lua::CLibraries::Graphics::Classes::Program::Use(lua_State* State) {
	glUseProgram(
		static_cast<Classes::Program*>(
			LuaHelper::CheckMetatable(State, 1, lua_upvalueindex(1))
		)->GLObject
	);
	return 0;
}


int Game::Lua::CLibraries::Graphics::Classes::Program::GetUniformIndex(lua_State* State) {
	lua_pushinteger(State,
		static_cast<lua_Integer>(
			glGetUniformLocation(
				static_cast<Classes::Program*>(luaL_checkudata(State, 1, "Program"))->GLObject,
				luaL_checkstring(State, 2)
			)
		)
	);
	return 1;
}

int Game::Lua::CLibraries::Graphics::Classes::Program::GetUniformBlockIndex(lua_State* State) {
	lua_pushinteger(State, static_cast<lua_Integer>(
			glGetUniformBlockIndex(
				static_cast<Classes::Program*>(luaL_checkudata(State, 1, "Program"))->GLObject,
				luaL_checkstring(State, 2)
			)
		)
	);
	return 1;
}

int Game::Lua::CLibraries::Graphics::Classes::Program::SetUniformBlockBinding(lua_State* State) {
	glUniformBlockBinding(
		static_cast<Classes::Program*>(luaL_checkudata(State, 1, "Program"))->GLObject,
		luaL_checkinteger(State, 2),	// UniformBlock Index
		luaL_checkinteger(State, 3)		// Bind To Index
	);
	return 0;
}



int Game::Lua::CLibraries::Graphics::Classes::Program::__gc(lua_State* State) {
	glDeleteProgram(static_cast<Classes::Program*>(luaL_checkudata(State, 1, "Program"))->GLObject);
	return 0;
}

/*
int Game::Lua::CLibraries::Graphics::Classes::Program::__eq(lua_State* State) {
	int Result;
	if (const Classes::Program* B = static_cast<Classes::Program*>(luaL_testudata(State, 2, "Program"))) {
		Result = static_cast<int>(static_cast<Classes::Program*>(luaL_checkudata(State, 1, "Program"))->GLObject == B->GLObject);
	} else {
		Result = static_cast<int>(false);
	}

	lua_settop(State, 0);
	lua_pushboolean(State, Result);
	return 1;
}*/