#ifndef PROGRAM_CLASS
#define PROGRAM_CLASS

#include "../define.h"

#include "OpenGLClassBase.hpp"

#include <iostream>

#include <glad/glad.h>

#include "../Vector2.hpp"
#include "../Vector3.hpp"

#include "Shader.hpp"
#include "Texture.hpp"

namespace {

	class UniformBase;
}

namespace OpenGL {
	namespace Classes {

		class Program : public OpenGL::Classes::OpenGLClassBase {
			friend UniformBase;

			GLuint ProgramObject;

#ifdef DEBUG
			void LinkedAssert(const char* Error) const;
#endif

		public:
			inline Program() = default;
			inline ~Program();

			inline void Create();

			inline void AttachShader(const OpenGL::Classes::Shader& Shader);
			inline void DetachShader(const OpenGL::Classes::Shader& Shader);

			inline GLint GetUniformLocation(const char* UniformName) const;
			inline GLuint GetUniformBlockIndex(const char* UniformBlockName) const;
			inline void SetUniformBlockBinding(const char* UniformBlockName, GLuint BlockBinding);
			inline void SetUniformBlockBinding(GLuint BlockIndex, GLuint BlockBinding);

			void Link();
			inline bool IsLinked() const;

			void DetatchAllShaders();

			inline void Use() const;

			inline GLuint GetID() const;
		};
	}
}

#ifdef DEBUG
void OpenGL::Classes::Program::LinkedAssert(const char* Error) const {
	if (!this->IsLinked()) {
		std::cerr << Error << std::endl;
		__debugbreak();
	}
}
#endif


void OpenGL::Classes::Program::Create() {
	this->ProgramObject = glCreateProgram();
#ifdef DEBUG
	this->Initialized = true;
#endif
}
OpenGL::Classes::Program::~Program() {

#ifdef DEBUG
	this->AssertInitialized("Attempted to deconstruct a OpenGL program before 'Create' was called.");
#endif
	glDeleteProgram(this->ProgramObject);
}


void OpenGL::Classes::Program::AttachShader(const OpenGL::Classes::Shader& Shader) {

#ifdef DEBUG
	this->AssertInitialized("Attempted to attach a shader from a OpenGL program before the programs' 'Create' function was called.");
#endif

	glAttachShader(this->ProgramObject, Shader.GetID());
}
void OpenGL::Classes::Program::DetachShader(const OpenGL::Classes::Shader& Shader) {

#ifdef DEBUG
	this->AssertInitialized("Attempted to detatch a shader from a OpenGL program before the programs' 'Create' function was called.");
#endif

	glDetachShader(this->ProgramObject, Shader.GetID());
}


GLint OpenGL::Classes::Program::GetUniformLocation(const char* UniformName) const {

#ifdef DEBUG
	this->AssertInitialized("Attempted to get a uniforms' location from a program before 'Create' was called.");
#endif

	return glGetUniformLocation(this->ProgramObject, UniformName);
}

GLuint OpenGL::Classes::Program::GetUniformBlockIndex(const char* UniformBlockName) const {

#ifdef DEBUG
	this->AssertInitialized("Attempted to get a UniformBlock's index from a program before 'Create' was called.");
#endif

	return glGetUniformBlockIndex(this->ProgramObject, UniformBlockName);
}

void OpenGL::Classes::Program::SetUniformBlockBinding(const char* UniformBlockName, GLuint BlockBinding) {

#ifdef DEBUG
	this->AssertInitialized("Attempted to set a UniformBlock's index from a program before 'Create' was called.");
#endif

	glUniformBlockBinding(this->ProgramObject, this->GetUniformBlockIndex(UniformBlockName), BlockBinding);
}

void OpenGL::Classes::Program::SetUniformBlockBinding(GLuint BlockIndex, GLuint BlockBinding) {

#ifdef DEBUG
	this->AssertInitialized("Attempted to set a UniformBlock's index from a program before 'Create' was called.");
#endif

	glUniformBlockBinding(this->ProgramObject, BlockIndex, BlockBinding);
}


void OpenGL::Classes::Program::Link() {

#ifdef DEBUG
	this->AssertInitialized("Attempted to link a OpenGL Program before 'Create' was called.");
#endif

	glLinkProgram(this->ProgramObject);
	if (!this->IsLinked()) {

		GLint ErrorLogLength;
		glGetProgramiv(this->ProgramObject, GL_INFO_LOG_LENGTH, &ErrorLogLength);

		std::unique_ptr<char[]> ErrorLog = std::make_unique<char[]>(ErrorLogLength);
		glGetProgramInfoLog(this->ProgramObject, ErrorLogLength, &ErrorLogLength, ErrorLog.get());

		Exceptions::ThrowExceptionExpression<Exceptions::ProgramLinkageError>("Failed to link OpenGL Shader Program. Error Log:\n", ErrorLog.get());
	}
}

bool OpenGL::Classes::Program::IsLinked() const {

#ifdef DEBUG
	this->AssertInitialized("Attempted to check if a OpenGL Program was linked before 'Create' was called.");
#endif

	GLint isLinked;
	glGetProgramiv(this->ProgramObject, GL_LINK_STATUS, &isLinked);
	return static_cast<bool>(isLinked);
}



void OpenGL::Classes::Program::DetatchAllShaders() {

#ifdef DEBUG
	this->AssertInitialized("Attempted to detatch all shaders from a OpenGL Program before 'Create' was called.");
#endif

	GLint AttachedShadersCount;
	GLuint* AttachedShaders;
	size_t ShaderIndex;

	glGetProgramiv(this->ProgramObject, GL_ATTACHED_SHADERS, &AttachedShadersCount);

	AttachedShaders = new GLuint[AttachedShadersCount];
	glGetAttachedShaders(this->ProgramObject, AttachedShadersCount, NULL, AttachedShaders);

	for (ShaderIndex = 0; ShaderIndex < AttachedShadersCount; ++ShaderIndex) {
		glDetachShader(this->ProgramObject, AttachedShaders[ShaderIndex]);
	}
}

void OpenGL::Classes::Program::Use() const {

#ifdef DEBUG
	this->AssertInitialized("Attempted to use a OpenGL Program before 'Create' was called.");
#endif

	glUseProgram(this->ProgramObject);
}


GLuint OpenGL::Classes::Program::GetID() const {
	return this->ProgramObject;
}

inline bool operator==(const OpenGL::Classes::Program& A, const OpenGL::Classes::Program& B) {
	return A.GetID() == B.GetID();
}



namespace OpenGL {
	namespace Program {

		// Helper function to quickly make a Shader Program. Compiles, attaches, and links the shaders and program. Detaches shaders after linkage.
		void QuickInitProgram(OpenGL::Classes::Program& Program, const char* VertexShaderSourceFile, const char* FragmentShaderSourceFile);
	}
}

void OpenGL::Program::QuickInitProgram(OpenGL::Classes::Program& Program, const char* VertexShaderSourceFile, const char* FragmentShaderSourceFile) {

	OpenGL::Classes::Shader VertexShader(GL_VERTEX_SHADER, VertexShaderSourceFile);
	OpenGL::Classes::Shader FragmentShader(GL_FRAGMENT_SHADER, FragmentShaderSourceFile);

	VertexShader.Compile();
	FragmentShader.Compile();

	Program.AttachShader(VertexShader);
	Program.AttachShader(FragmentShader);

	Program.Link();

	Program.DetachShader(VertexShader);
	Program.DetachShader(FragmentShader);
}


namespace {

	class UniformBase {
	protected:
		GLint UniformID;

	public:
		inline UniformBase() {}
		inline UniformBase(const char* UniformName, OpenGL::Classes::Program& ParentProgram);

		inline void UpdateUniformLocation(const char* UniformName, OpenGL::Classes::Program& ParentProgram);

		inline GLint GetID() const;
	};

	UniformBase::UniformBase(const char* UniformName, OpenGL::Classes::Program& ParentProgram) {
		this->UpdateUniformLocation(UniformName, ParentProgram);
	}

	void UniformBase::UpdateUniformLocation(const char* UniformName, OpenGL::Classes::Program& ParentProgram) {
		this->UniformID = ParentProgram.GetUniformLocation(UniformName);
	}

	GLint UniformBase::GetID() const {
		return this->UniformID;
	}
}

namespace OpenGL {
	namespace Classes {

		template<typename T>
		class Uniform;

		template<>
		class Uniform<GLint> : public UniformBase {
		public:
			inline Uniform() {}
			inline Uniform(const char* UniformName, OpenGL::Classes::Program& ParentProgram) : UniformBase(UniformName, ParentProgram) {}

			inline void SetValue(GLint Value) {
				glUniform1i(this->UniformID, Value);
			}
		};

		template<>
		class Uniform<GLuint> : public UniformBase {
		public:
			inline Uniform() {}
			inline Uniform(const char* UniformName, OpenGL::Classes::Program& ParentProgram) : UniformBase(UniformName, ParentProgram) {}

			inline void SetValue(GLuint Value) {
				glUniform1ui(this->UniformID, Value);
			}
		};

		template<>
		class Uniform<GLfloat> : public UniformBase {
		public:
			inline Uniform() {}
			inline Uniform(const char* UniformName, OpenGL::Classes::Program& ParentProgram) : UniformBase(UniformName, ParentProgram) {}

			inline void SetValue(GLfloat Value) {
				glUniform1f(this->UniformID, Value);
			}
		};


		template<>
		class Uniform<Math::Vector2> : public UniformBase {
		public:
			inline Uniform() {}
			inline Uniform(const char* UniformName, OpenGL::Classes::Program& ParentProgram) : UniformBase(UniformName, ParentProgram) {}

			inline void SetValue(const Math::Vector2& Vector) {
				glUniform2f(this->UniformID, Vector.X, Vector.Y);
			}
		};

		template<>
		class Uniform<Math::Vector3> : public UniformBase {
		public:
			inline Uniform() {}
			inline Uniform(const char* UniformName, OpenGL::Classes::Program& ParentProgram) : UniformBase(UniformName, ParentProgram) {}

			inline void SetValue(const Math::Vector3& Vector) {
				glUniform3f(this->UniformID, Vector.X, Vector.Y, Vector.Z);
			}
		};


		template<>
		class Uniform<Math::IVector2> : public UniformBase {
		public:
			inline Uniform() {}
			inline Uniform(const char* UniformName, OpenGL::Classes::Program& ParentProgram) : UniformBase(UniformName, ParentProgram) {}

			inline void SetValue(const Math::IVector2& Vector) {
				//glUniform1iv(this->UniformID, 2, reinterpret_cast<const GLint*>(&Vector));
				glUniform2i(this->UniformID, Vector.X, Vector.Y);
			}
		};


		template<>
		class Uniform<OpenGL::Classes::Texture> : public Uniform<GLint> {
		public:
			inline Uniform() {}
			inline Uniform(const char* UniformName, OpenGL::Classes::Program& ParentProgram) : Uniform<GLint>(UniformName, ParentProgram) {}
		};
	}
}

#endif