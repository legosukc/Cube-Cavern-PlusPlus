#include "OpenGL_BufferBase.hpp"

#include "../../../define.h"

#include <SDL3/SDL_platform_defines.h>

#ifdef SDL_PLATFORM_VITA
#include <GLES2/gl2.h>
#include "../../GLES2_typedefs.h"
#else
#include <SDL3/SDL_opengl.h>
#endif

namespace Game::Graphics::OpenGLFunctions {
    PFNGLGENBUFFERSPROC glGenBuffers = NULL;
    PFNGLDELETEBUFFERSPROC glDeleteBuffers = NULL;

    PFNGLBUFFERDATAPROC glBufferData = NULL;
    PFNGLBUFFERSUBDATAPROC glBufferSubData = NULL;

    PFNGLBINDBUFFERPROC glBindBuffer = NULL;
}

template <GLenum BufferTarget>
Game::Graphics::Classes::BufferBase_OpenGL<BufferTarget>::~BufferBase_OpenGL() {
    unlikely_branch if (this->GLObject == 0) {
#ifdef DEBUG_BUILD
        std::cerr << "Attempted to free an unallocated OpenGL buffer."
                  << std::endl;
        __debugbreak();
#endif
        return;
    }
    Game::Graphics::OpenGLFunctions::glDeleteBuffers(1, &this->GLObject);
}

template <GLenum BufferTarget>
void Game::Graphics::Classes::BufferBase_OpenGL<BufferTarget>::Bind() {
    Game::Graphics::OpenGLFunctions::glBindBuffer(BufferTarget, this->GLObject);
}

template Game::Graphics::Classes::BufferBase_OpenGL<
    GL_ARRAY_BUFFER>::~BufferBase_OpenGL();
template void
    Game::Graphics::Classes::BufferBase_OpenGL<GL_ARRAY_BUFFER>::Bind();

template Game::Graphics::Classes::BufferBase_OpenGL<
    GL_ELEMENT_ARRAY_BUFFER>::~BufferBase_OpenGL();
template void
    Game::Graphics::Classes::BufferBase_OpenGL<GL_ELEMENT_ARRAY_BUFFER>::Bind();

template Game::Graphics::Classes::BufferBase_OpenGL<
    GL_UNIFORM_BUFFER>::~BufferBase_OpenGL();
template void
    Game::Graphics::Classes::BufferBase_OpenGL<GL_UNIFORM_BUFFER>::Bind();