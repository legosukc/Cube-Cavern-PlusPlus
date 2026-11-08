#include "VertexBuffer.hpp"

#include <SDL3/SDL_platform_defines.h>

#ifdef SDL_PLATFORM_VITA
#include <GLES2/gl2.h>
#include "../../GLES2_typedefs.h"
#else
#include <SDL3/SDL_opengl.h>
#include <SDL3/SDL_opengl_glext.h>
#endif

namespace Game::Graphics::VertexBuffer {

    Game::Graphics::Classes::VertexBuffer* (*Create)() = NULL;
    void (*CreateBulk)(
        size_t CreateAmount,
        Game::Graphics::Classes::VertexBuffer* VertexBuffers[]) = NULL;
    void (*Unbind)() = NULL;
}

bool Game::Graphics::Classes::VertexBuffer_OpenGL::IsBound() const {
    // TODO: FIX THIS!!!!
    return true;
}