#ifndef GRAPHICS_BASECLASSDEFINITIONS_H
#define GRAPHICS_BASECLASSDEFINITIONS_H

#include "../../define.h"

#include <SDL3/SDL_opengl.h>

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
#endif