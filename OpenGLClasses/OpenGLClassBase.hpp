#ifndef OPENGL_BASE_CLASS
#define OPENGL_BASE_CLASS

#include "../define.h"

#include <iostream>



namespace OpenGL {
	namespace Classes {
		class OpenGLClassBase;
	}
}


class OpenGL::Classes::OpenGLClassBase {
protected:
#ifdef DEBUG
	mutable bool Initialized = false;
	inline void AssertInitialized(const char* Error) const;
#endif
public:
	inline OpenGLClassBase() = default;
	inline ~OpenGLClassBase() = default;

	inline void Create();
};


#ifdef DEBUG

void OpenGL::Classes::OpenGLClassBase::AssertInitialized(const char* Error) const {
	if (!this->Initialized) {
		std::cerr << Error << std::endl;
		__debugbreak();
	}
}
#endif

#endif