#include "OpenGL_UniformBuffer.hpp"

#include <SDL3/SDL_platform_defines.h>
#ifdef SDL_PLATFORM_VITA
#include <GLES2/gl2.h>
#include "../../../../GLES2_typedefs.h"
#else
#include <SDL3/SDL_opengl.h>
#include <SDL3/SDL_opengl_glext.h>
#endif

namespace Game::Graphics::OpenGLFunctions {
#ifndef SDL_PLATFORM_VITA
    PFNGLBINDBUFFERBASEPROC glBindBufferBase = NULL;
    PFNGLBINDBUFFERRANGEPROC glBindBufferRange = NULL;
#endif
}

#ifndef SDL_PLATFORM_VITA
void Game::Graphics::Classes::UniformBuffer_OpenGL::BindBase(GLuint Index) {
    Game::Graphics::OpenGLFunctions::glBindBufferBase(GL_UNIFORM_BUFFER, Index,
                                                      this->GLObject);
}

bool Game::Graphics::Classes::UniformBuffer_OpenGL::IsBound() const {
    // TODO: FIX THIS!!!!
    return true;
}
#endif