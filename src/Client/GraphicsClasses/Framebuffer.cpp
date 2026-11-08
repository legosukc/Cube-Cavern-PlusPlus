#include "Framebuffer.hpp"

#include "../../define.h"

#include <cstddef>

#ifdef SDL_PLATFORM_VITA
#include <GLES2/gl2.h>
#include "../../GLES2_typedefs.h"
#else
#include <SDL3/SDL_opengl.h>
#include <SDL3/SDL_opengl_glext.h>
#endif

#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_video.h>

namespace Game::Graphics::OpenGLFunctions {
    PFNGLGENFRAMEBUFFERSPROC glGenFramebuffers = NULL;
    PFNGLDELETEFRAMEBUFFERSPROC glDeleteFramebuffers = NULL;

    PFNGLBINDFRAMEBUFFERPROC glBindFramebuffer = NULL;
}

namespace Game::Graphics::Framebuffer {

    Game::Graphics::Classes::Framebuffer* (*Create)() = NULL;
    void (*CreateBulk)(size_t CreateAmount,
                              Game::Graphics::Classes::Framebuffer* Textures[]) = NULL;
    void (*Unbind)() = NULL;
}

Game::Graphics::Classes::Framebuffer::~Framebuffer() = default;

void Game::Graphics::Classes::Framebuffer::Bind() {}
bool Game::Graphics::Classes::Framebuffer::IsBound() const {
    return false;
}

Game::Graphics::Classes::Framebuffer_OpenGL::~Framebuffer_OpenGL() {
    if (this->GLObject == 0) {
#ifdef DEBUG_BUILD
        std::cerr << "Attempted to free a unallocated OpenGL Framebuffer."
                  << std::endl;
        __debugbreak();
#else
        return;
#endif
    }
    Game::Graphics::OpenGLFunctions::glDeleteFramebuffers(1, &this->GLObject);
}

void Game::Graphics::Classes::Framebuffer_OpenGL::Bind() {
    Game::Graphics::OpenGLFunctions::glBindFramebuffer(GL_FRAMEBUFFER,
                                                       this->GLObject);
}

bool Game::Graphics::Classes::Framebuffer_OpenGL::IsBound() const {
    // TODO: FIX THIS!!!!
    return true;
}

Game::Graphics::Classes::Framebuffer*
    Game::Graphics::Framebuffer::etc::_Framebuffer_Create_OpenGL() {
    Game::Graphics::Classes::Framebuffer_OpenGL* NewFramebuffer =
        new Game::Graphics::Classes::Framebuffer_OpenGL;

    Game::Graphics::OpenGLFunctions::glGenFramebuffers(
        1, &NewFramebuffer->GLObject);

    return reinterpret_cast<Game::Graphics::Classes::Framebuffer*>(
        NewFramebuffer);
}

void Game::Graphics::Framebuffer::etc::_Framebuffer_CreateBulk_OpenGL(
    size_t CreateAmount,
    Game::Graphics::Classes::Framebuffer* Textures[]) {
    GLuint* TextureArray = new GLuint[CreateAmount];
    for (size_t i = 0; i < CreateAmount; ++i) {
        TextureArray[i] = Textures[i]->GLObject;
    }

    Game::Graphics::OpenGLFunctions::glGenFramebuffers(CreateAmount,
                                                       TextureArray);

    delete[] TextureArray;
}

void Game::Graphics::Framebuffer::etc::_Framebuffer_Unbind_OpenGL() {
    Game::Graphics::OpenGLFunctions::glBindFramebuffer(GL_FRAMEBUFFER, 0);
}