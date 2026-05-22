#pragma once

#include "../../define.h"

#include <iostream>


namespace OpenGL::Classes {
	class OpenGLClassBase;
}


class OpenGL::Classes::OpenGLClassBase {
public:
	inline void Create();

#ifdef DEBUG_BUILD
protected:
	mutable bool Initialized = false;

	inline void AssertInitialized(const char* Error) const {
		if (!this->Initialized) {
			std::cerr << Error << std::endl;
			__debugbreak();
		}
	}
#endif
};