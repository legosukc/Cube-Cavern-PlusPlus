#pragma once

#include "../../define.h"

#include <SDL3/SDL_opengl.h>

namespace Game::Graphics::Classes {
		
	class OpenGLClassBase {
	public:
		GLuint GLObject = 0;

		constexpr bool operator==(const OpenGLClassBase B) const;
	};
}