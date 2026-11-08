#include "VertexArray.hpp"

#include <SDL3/SDL_platform_defines.h>
#include <SDL3/SDL_video.h>

#ifdef SDL_PLATFORM_VITA
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include "../../GLES2_typedefs.h"
#else
#include <SDL3/SDL_opengl.h>
#include <SDL3/SDL_opengl_glext.h>
#endif

namespace Game::Graphics::OpenGLFunctions {
    void (*glGenVertexArrays)(GLsizei n, GLuint* arrays) = NULL;
    void (*glDeleteVertexArrays)(GLsizei n, const GLuint* arrays) = NULL;

    void (*glBindVertexArray)(GLuint array) = NULL;

    PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray = NULL;
    PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray = NULL;

    PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer = NULL;
}

namespace Game::Graphics::VertexArray {
    Game::Graphics::Classes::VertexArray* (*Create)() = NULL;
    void (*CreateBulk)(
        size_t CreateAmount,
        Game::Graphics::Classes::VertexArray* VertexArrays[]) = NULL;
    void (*Unbind)() = NULL;

}

Game::Graphics::Classes::VertexArray::~VertexArray() = default;

void Game::Graphics::Classes::VertexArray::Bind() {}

bool Game::Graphics::Classes::VertexArray::IsBound() const {
    return false;
}

Game::Graphics::Classes::VertexArray_OpenGL::~VertexArray_OpenGL() {
    if (this->GLObject == 0) {
#ifdef DEBUG_BUILD
        std::cerr << "Attempted to free a unallocated OpenGL "
                     "VertexArray (VertexArray_OpenGL)."
                  << std::endl;
        __debugbreak();
#endif
        return;
    }
    Game::Graphics::OpenGLFunctions::glDeleteVertexArrays(1, &this->GLObject);
}

void Game::Graphics::Classes::VertexArray_OpenGL::Bind() {
    Game::Graphics::OpenGLFunctions::glBindVertexArray(this->GLObject);
}

bool Game::Graphics::Classes::VertexArray_OpenGL::IsBound() const {
    // TODO: FIX THIS!!!!
    return true;
}