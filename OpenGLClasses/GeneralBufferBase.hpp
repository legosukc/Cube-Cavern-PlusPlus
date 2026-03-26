#ifndef GENERAL_BUFFER_BASE_CLASS
#define GENERAL_BUFFER_BASE_CLASS

#include <glad/glad.h>

#include "OpenGLClassBase.hpp"


namespace OpenGL {
	namespace Classes {

		template<GLenum BufferEnum>
		class GeneralBufferBase : public OpenGL::Classes::OpenGLClassBase {
		protected:
			GLuint Buffer;

		public:
			inline GeneralBufferBase() = default;
			inline ~GeneralBufferBase();

			inline void Bind();
			inline void Create();

			inline GLuint GetID() const;
		};
	}
}


template<GLenum BufferEnum>
OpenGL::Classes::GeneralBufferBase<BufferEnum>::~GeneralBufferBase() {

#ifdef DEBUG
	this->AssertInitialized("Attempted to destroy a OpenGL buffer before 'Create' was called.");
#endif
	glDeleteBuffers(1, &this->Buffer);
}


template<GLenum BufferEnum>
void OpenGL::Classes::GeneralBufferBase<BufferEnum>::Create() {
	glGenBuffers(1, &this->Buffer);
#ifdef DEBUG
	this->Initialized = true;
#endif
}


template<GLenum BufferEnum>
void OpenGL::Classes::GeneralBufferBase<BufferEnum>::Bind() {

#ifdef DEBUG
	this->AssertInitialized("Attempted to bind a OpenGL buffer before 'Create' was called.");
#endif
	glBindBuffer(BufferEnum, this->Buffer);
}

template<GLenum BufferEnum>
GLuint OpenGL::Classes::GeneralBufferBase<BufferEnum>::GetID() const {
	return this->Buffer;
}


template<GLenum BufferEnum>
inline bool operator==(const OpenGL::Classes::GeneralBufferBase<BufferEnum>& A, const OpenGL::Classes::GeneralBufferBase<BufferEnum>& B) {
	return A.GetID() == B.GetID();
}

#endif