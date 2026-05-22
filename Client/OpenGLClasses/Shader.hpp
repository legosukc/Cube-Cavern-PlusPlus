#pragma once

#include <glad/glad.h>
#include <SDL3/SDL.h>

#include <memory>

#include "OpenGLClassBase.hpp"

#include "../../FunctionHeaders/File.hpp"


namespace OpenGL {

	namespace Classes {

		class Shader {

			GLuint ShaderObject;
			GLenum Type;

		public:
			Shader(GLenum ShaderType);
			Shader(GLenum ShaderType, const char* ShaderPath);
			inline ~Shader();

			inline void SetSource(const char* ShaderSource);
			void Compile();

			inline GLuint GetID() const;
		};
	}
}


OpenGL::Classes::Shader::Shader(GLenum ShaderType) {

	this->ShaderObject = glCreateShader(ShaderType);
	this->Type = ShaderType;
}

OpenGL::Classes::Shader::Shader(GLenum ShaderType, const char* ShaderPath) : Shader(ShaderType) {

	std::unique_ptr<char[]> ShaderSource = File::ReadTextFile(ShaderPath);
	this->SetSource(ShaderSource.get());
}

OpenGL::Classes::Shader::~Shader() {
	glDeleteShader(this->ShaderObject);
}


void OpenGL::Classes::Shader::SetSource(const char* ShaderSource) {
	glShaderSource(this->ShaderObject, 1, &ShaderSource, nullptr);
}

void OpenGL::Classes::Shader::Compile() {
	glCompileShader(this->ShaderObject);

	GLint isCompiled;
	glGetShaderiv(this->ShaderObject, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE) {

		GLint ErrorLogLength;
		glGetShaderiv(this->ShaderObject, GL_INFO_LOG_LENGTH, &ErrorLogLength);

		std::unique_ptr<char[]> ErrorLog = std::make_unique<char[]>(ErrorLogLength);
		glGetShaderInfoLog(this->ShaderObject, ErrorLogLength, &ErrorLogLength, ErrorLog.get());

		this->~Shader();
		Exceptions::ThrowExceptionExpression<Exceptions::ShaderCompilationError>("Failed to compile shader. Error Log:\n", ErrorLog.get());
	}
}


GLuint OpenGL::Classes::Shader::GetID() const {
	return this->ShaderObject;
}



namespace OpenGL {
	namespace Shader {

		
	}
}