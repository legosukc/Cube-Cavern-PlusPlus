#pragma once

#include <SDL3/SDL_platform_defines.h>

#ifdef SDL_PLATFORM_VITA
#include <GLES2/gl2.h>
#else
#include <SDL3/SDL_opengl.h>
#endif

namespace Game::Graphics::Classes {
		
	class OpenGLClassBase {
	public:
		GLuint GLObject = 0;

		constexpr bool operator==(const OpenGLClassBase B) const;
	};
}

constexpr bool Game::Graphics::Classes::OpenGLClassBase::operator==(
    const Game::Graphics::Classes::OpenGLClassBase B) const {
    return this->GLObject == B.GLObject;
}