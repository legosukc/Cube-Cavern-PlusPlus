#pragma once

#include <glad/glad.h>

#include "GeneralBufferBase.hpp"

#include "Program.hpp"



namespace OpenGL {
	namespace Classes {

		class UniformBuffer : public OpenGL::Classes::GeneralBufferBase<GL_UNIFORM_BUFFER> {
		public:
			inline UniformBuffer() = default;
			inline ~UniformBuffer() = default;

			inline void BindBase(GLuint BlockIndex);
			inline void BindRange(GLuint BlockIndex, GLintptr Offset, GLsizei Size);
		};
	}

	namespace UniformBuffer {

		inline void Unbind();

		GLuint ReservedBlockIndexes = 0;

		inline void SendData(GLsizeiptr Size, const void* Data, GLenum Usage = GL_STATIC_DRAW);
		inline void SendPointerData(GLintptr Offset, GLsizeiptr Size, const void* Data);

		inline void Allocate(GLsizeiptr Size, GLenum Usage = GL_STATIC_DRAW);
	}
}

void OpenGL::Classes::UniformBuffer::BindBase(GLuint BlockIndex) {
	glBindBufferBase(GL_UNIFORM_BUFFER, OpenGL::UniformBuffer::ReservedBlockIndexes + BlockIndex, this->Buffer);
}
void OpenGL::Classes::UniformBuffer::BindRange(GLuint BlockIndex, GLintptr Offset, GLsizei Size) {
	glBindBufferRange(GL_UNIFORM_BUFFER, OpenGL::UniformBuffer::ReservedBlockIndexes + BlockIndex, this->Buffer, Offset, Size);
}



void OpenGL::UniformBuffer::Unbind() {
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}


void OpenGL::UniformBuffer::SendData(GLsizeiptr Size, const void* Data, GLenum Usage) {
	glBufferData(GL_UNIFORM_BUFFER, Size, Data, Usage);
}

void OpenGL::UniformBuffer::SendPointerData(GLintptr Offset, GLsizeiptr Size, const void* Data) {
	glBufferSubData(GL_UNIFORM_BUFFER, Offset, Size, Data);
}


void OpenGL::UniformBuffer::Allocate(GLsizeiptr Size, GLenum Usage) {
	OpenGL::UniformBuffer::SendData(Size, nullptr, Usage);
}