#pragma once

#include <glad/glad.h>


//#include "../Statistics.hpp"


namespace Game::Graphics::Classes {
		
	class OpenGLClassBase {
	public:
		GLuint GLObject = 0;

		constexpr bool operator==(const OpenGLClassBase& B) const {
			return this->GLObject == B.GLObject;
		}
	};

	template<GLenum BufferType>
	class BufferBase : public Game::Graphics::Classes::OpenGLClassBase {
	public:
		~BufferBase() {
			if (this->GLObject == 0) {
#ifdef DEBUG_BUILD
				std::cerr << "Attempted to free a unallocated OpenGL Buffer." << std::endl;
				__debugbreak();
#else
				return;
#endif
			}
			glDeleteBuffers(1, &this->GLObject);
		}

		inline void Create() {
			glGenBuffers(1, &this->GLObject);
		}

		inline void Bind(GLenum BindTarget = BufferType) {
			glBindBuffer(BindTarget, this->GLObject);
		}

		inline bool IsBound(GLenum BindTarget = BufferType) const {
			return;
		}
	};


	class VertexArray;
	class VertexBuffer : public BufferBase<GL_ARRAY_BUFFER> {};
	class ElementBuffer : public BufferBase<GL_ELEMENT_ARRAY_BUFFER> {};
	class UniformBuffer;

	class Program;
	class Shader;

	class Texture;
}



class Game::Graphics::Classes::UniformBuffer : public Game::Graphics::Classes::BufferBase<GL_UNIFORM_BUFFER> {
public:
};




class Game::Graphics::Classes::Program : public Game::Graphics::Classes::OpenGLClassBase {
public:
	~Program() {
		if (this->GLObject == 0) {
			return;
		}
		glDeleteProgram(this->GLObject);
	}

	inline void Create() {
		this->GLObject = glCreateProgram();
	}
};


class Game::Graphics::Classes::Shader {
public:
	inline ~Shader() {
		glDeleteShader(this->GLObject);
	}

	inline void Create(GLenum ShaderType) {
		this->GLObject = glCreateShader(ShaderType);
	}

	GLint GLObject;
};




class Game::Graphics::Classes::Texture : public Game::Graphics::Classes::OpenGLClassBase {
public:
	~Texture() {
		if (this->GLObject == 0) {
			return;
		}
		glDeleteTextures(1, &this->GLObject);
	}

	inline void Create() {
		glGenTextures(1, &this->GLObject);
	}
};