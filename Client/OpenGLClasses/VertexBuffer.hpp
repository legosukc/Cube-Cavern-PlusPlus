#ifndef VERTEXBUFFER_CLASS
#define VERTEXBUFFER_CLASS

#include <glad/glad.h>

#include "GeneralBufferBase.hpp"


namespace OpenGL {
	namespace Classes {

		using VertexBuffer = OpenGL::Classes::GeneralBufferBase<GL_ARRAY_BUFFER>;
	}
}




namespace OpenGL {
	namespace VertexBuffer {

		inline void Unbind();

		inline void SendData(GLsizeiptr Size, const void* Data, GLenum Usage = GL_STATIC_DRAW);
		inline void SendPointerData(GLintptr Offset, GLsizeiptr Size, const void* Data);

		inline void Allocate(GLsizeiptr Size, GLenum Usage = GL_STATIC_DRAW);
	}
}

void OpenGL::VertexBuffer::Unbind() {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void OpenGL::VertexBuffer::SendData(GLsizeiptr Size, const void* Data, GLenum Usage) {
	glBufferData(GL_ARRAY_BUFFER, Size, Data, Usage);
}

void OpenGL::VertexBuffer::SendPointerData(GLintptr Offset, GLsizeiptr Size, const void* Data) {
	glBufferSubData(GL_ARRAY_BUFFER, Offset, Size, Data);
}


void OpenGL::VertexBuffer::Allocate(GLsizeiptr Size, GLenum Usage) {
	OpenGL::VertexBuffer::SendData(Size, nullptr, Usage);
}

#endif