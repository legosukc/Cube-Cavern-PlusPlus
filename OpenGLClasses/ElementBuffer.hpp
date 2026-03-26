#ifndef ELEMENTBUFFER_CLASS
#define ELEMENTBUFFER_CLASS

#include <glad/glad.h>

#include "GeneralBufferBase.hpp"


namespace OpenGL {
	namespace Classes {
		using ElementBuffer = OpenGL::Classes::GeneralBufferBase<GL_ELEMENT_ARRAY_BUFFER>;
	}
}



namespace OpenGL {
	namespace ElementBuffer {

		inline void Unbind();

		inline void SendData(GLsizeiptr Size, const void* Data, GLenum Usage = GL_STATIC_DRAW);
		inline void SendPointerData(GLintptr Offset, GLsizeiptr Size, const void* Data);

		inline void Allocate(GLsizeiptr Size, GLenum Usage = GL_STATIC_DRAW);
	}
}

void OpenGL::ElementBuffer::Unbind() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


void OpenGL::ElementBuffer::SendData(GLsizeiptr Size, const void* Data, GLenum Usage) {
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, Size, Data, Usage);
}

void OpenGL::ElementBuffer::SendPointerData(GLintptr Offset, GLsizeiptr Size, const void* Data) {
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, Offset, Size, Data);
}


void OpenGL::ElementBuffer::Allocate(GLsizeiptr Size, GLenum Usage) {
	OpenGL::ElementBuffer::SendData(Size, nullptr, Usage);
}

#endif