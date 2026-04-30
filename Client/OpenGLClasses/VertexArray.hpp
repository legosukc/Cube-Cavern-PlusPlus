#ifndef VERTEXARRAY_CLASS
#define VERTEXARRAY_CLASS

#include "../define.h"

#include <glad/glad.h>

#include "OpenGLClassBase.hpp"


namespace OpenGL {
	namespace Classes {

		class VertexArray : public OpenGL::Classes::OpenGLClassBase {

			GLuint Buffer;

		public:
			inline VertexArray() = default;
			inline ~VertexArray();

			inline void Bind();
			inline void Create();

			inline GLuint GetID() const;
		};
	}
}


OpenGL::Classes::VertexArray::~VertexArray() {

#ifdef DEBUG
	this->AssertInitialized("The deconstructor to a OpenGL VertexArray was called before 'Create' was called.");
#endif
	glDeleteVertexArrays(1, &this->Buffer);
}

void OpenGL::Classes::VertexArray::Create() {
	glGenVertexArrays(1, &this->Buffer);
#ifdef DEBUG
	this->Initialized = true;
#endif
}


void OpenGL::Classes::VertexArray::Bind() {

#ifdef DEBUG
	this->AssertInitialized("Attempted to bind a OpenGL VertexArray before 'Create' was called.");
#endif
	glBindVertexArray(this->Buffer);
}


GLuint OpenGL::Classes::VertexArray::GetID() const {
	return this->Buffer;
}



namespace OpenGL {
	namespace VertexArray {

		inline void Unbind();

		inline void VertexAttributePointer(GLuint Index, GLint Length, GLenum AttributeType, bool Normalized, GLsizei StrideSize, size_t Offset);

		inline void DefineSingleAttribute(GLuint Index, GLenum AttributeType, GLsizei StrideSize, size_t Offset);
		inline void DefineVector2Attribute(GLuint Index, GLenum AttributeType, bool Normalized, GLsizei StrideSize, size_t Offset);

		constexpr void SetAttributeEnabled(GLuint Index, bool Enabled);
	}
}

void OpenGL::VertexArray::Unbind() {
	glBindVertexArray(0);
}

void OpenGL::VertexArray::VertexAttributePointer(GLuint Index, GLint Length, GLenum AttributeType, bool Normalized, GLsizei StrideSize, size_t Offset) {
	glVertexAttribPointer(Index, Length, AttributeType, static_cast<GLboolean>(Normalized), StrideSize, (void*)Offset);
}

void OpenGL::VertexArray::DefineSingleAttribute(GLuint Index, GLenum AttributeType, GLsizei StrideSize, size_t Offset) {
	OpenGL::VertexArray::VertexAttributePointer(Index, 1, AttributeType, false, StrideSize, Offset);
}

void OpenGL::VertexArray::DefineVector2Attribute(GLuint Index, GLenum AttributeType, bool Normalized, GLsizei StrideSize, size_t Offset) {
	OpenGL::VertexArray::VertexAttributePointer(Index, 2, AttributeType, Normalized, StrideSize, Offset);
}

constexpr void OpenGL::VertexArray::SetAttributeEnabled(GLuint Index, bool Enabled) {

	if (Enabled) {
		glEnableVertexAttribArray(Index);
	} else {
		glDisableVertexAttribArray(Index);
	}
}

#endif