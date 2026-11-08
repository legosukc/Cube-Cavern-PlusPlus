#include "IndexBuffer.hpp"

#include <SDL3/SDL_platform_defines.h>

#ifdef SDL_PLATFORM_VITA
#include <GLES2/gl2.h>
#include "../../GLES2_typedefs.h"
#else
#include <SDL3/SDL_opengl.h>
#endif

namespace Game::Graphics::IndexBuffer {

    Game::Graphics::Classes::IndexBuffer* (*Create)() = NULL;
    void (*CreateBulk)(
        size_t CreateAmount,
        Game::Graphics::Classes::IndexBuffer* IndexBuffers[]) = NULL;
    void (*Unbind)() = NULL;
}

bool Game::Graphics::Classes::IndexBuffer_OpenGL::IsBound() const {
    // TODO: FIX THIS!!!!
    return true;
}