#pragma once

#include <SDL3/SDL_opengl_glext.h>

namespace Game::Graphics::Classes {
		
	class OpenGLClassBase {
	public:
		GLuint GLObject = 0;

		constexpr bool operator==(const OpenGLClassBase& B) const {
			return this->GLObject == B.GLObject;
		}
	};
}