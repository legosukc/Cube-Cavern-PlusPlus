#ifndef FRAMEBUFFER_CLASS
#define FRAMEBUFFER_CLASS

#include <glad/glad.h>

#include "../Exceptions.hpp"

#include "Texture.hpp"
#include "OpenGLClassBase.hpp"

namespace OpenGL {
	namespace Classes {
		class Framebuffer;
	}
}



namespace {
	static OpenGL::Classes::Framebuffer* _DefaultFramebuffer = nullptr;
	static GLuint _BoundFramebufferID = 0;
}

namespace OpenGL {
	namespace Framebuffer {

		void SetDefaultFramebuffer(OpenGL::Classes::Framebuffer* Framebuffer);

		inline void Unbind(GLenum Target = GL_FRAMEBUFFER);

		void CheckBoundBufferCompleteness(GLenum Target = GL_FRAMEBUFFER);

		void BindDefaultFramebuffer(GLenum Target = GL_FRAMEBUFFER);
	}
}


class OpenGL::Classes::Framebuffer : public OpenGL::Classes::OpenGLClassBase {

	GLuint FramebufferObject;
	mutable bool IsDefaultFramebuffer = false;
	mutable GLenum BoundTo = 0;

public:
	inline Framebuffer() = default;
	inline ~Framebuffer();

	// This function expects the Framebuffer class to be initialized with "Create" before it is called. No checks exist to confirm it's initialized.
	inline void Bind(GLenum Target = GL_FRAMEBUFFER);
			
	inline void Create();
	static Framebuffer CreateConstruct();

	friend void OpenGL::Framebuffer::SetDefaultFramebuffer(OpenGL::Classes::Framebuffer* Framebuffer);
};

OpenGL::Classes::Framebuffer::~Framebuffer() {

#ifdef DEBUG
	this->AssertInitialized("You fucked up. You forgot to call 'Create' on a Framebuffer and it's deconstructor was called. Shame on you, bastard.");
#endif

	if (::_DefaultFramebuffer->FramebufferObject == this->FramebufferObject) {
		::_DefaultFramebuffer = nullptr;
	}
	glDeleteFramebuffers(1, &this->FramebufferObject);
}


void OpenGL::Classes::Framebuffer::Create() {
	glGenFramebuffers(1, &this->FramebufferObject);
#ifdef DEBUG
	this->Initialized = true;
#endif
}

OpenGL::Classes::Framebuffer OpenGL::Classes::Framebuffer::CreateConstruct() {
	OpenGL::Classes::Framebuffer Buffer;
	Buffer.Create();
	return Buffer;
}


void OpenGL::Classes::Framebuffer::Bind(GLenum Target) {

#ifdef DEBUG
	if (!this->Initialized) {
		std::cerr << "You fucked up. You forgot to call 'Create' before using a Framebuffer. Shame on you, bastard." << std::endl;
		__debugbreak();
	}
#endif

	glBindFramebuffer(Target, this->FramebufferObject);
	this->BoundTo = Target;
}


void OpenGL::Framebuffer::SetDefaultFramebuffer(OpenGL::Classes::Framebuffer* Framebuffer) {

	if (Framebuffer != nullptr) {
		Framebuffer->IsDefaultFramebuffer = true;
		::_BoundFramebufferID = Framebuffer->FramebufferObject;
	} else {
		::_DefaultFramebuffer->IsDefaultFramebuffer = false;
		::_BoundFramebufferID = 0;
	}

	::_DefaultFramebuffer = Framebuffer;
}


void OpenGL::Framebuffer::Unbind(GLenum Target) {
	glBindFramebuffer(Target, 0);
}


void OpenGL::Framebuffer::CheckBoundBufferCompleteness(GLenum Target) {

	const GLenum FramebufferStatus = glCheckFramebufferStatus(Target);
	if (FramebufferStatus != GL_FRAMEBUFFER_COMPLETE) {
		Exceptions::ThrowException<Exceptions::FramebufferUncompletenessError>(Exceptions::FramebufferUncompletenessError("Framebuffer isn't complete!", FramebufferStatus));
	}
}


void OpenGL::Framebuffer::BindDefaultFramebuffer(GLenum Target) {
	if (::_DefaultFramebuffer == nullptr) {
		glBindFramebuffer(Target, 0);
	} else {
		::_DefaultFramebuffer->Bind();
	}
}

#endif