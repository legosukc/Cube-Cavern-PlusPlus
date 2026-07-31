#ifndef GRAPHICS_FRAMEBUFFER_H
#define GRAPHICS_FRAMEBUFFER_H

#include "../../define.h"

#include <cstddef>

#include <SDL3/SDL_opengl.h>
#include <SDL3/SDL_opengl_glext.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_video.h>


#include "BaseClassDefinitions.hpp"

namespace Game::Graphics::OpenGLFunctions {
    PFNGLGENFRAMEBUFFERSPROC glGenFramebuffers;
    PFNGLDELETEFRAMEBUFFERSPROC glDeleteFramebuffers;

    PFNGLBINDFRAMEBUFFERPROC glBindFramebuffer;
}

namespace Game::Graphics::Classes {

    class Framebuffer : public Game::Graphics::Classes::OpenGLClassBase {
       public:
        virtual ~Framebuffer();

        virtual void Bind();
        virtual bool IsBound() const;
    };

    class Framebuffer_OpenGL : public Framebuffer {
       public:
        virtual ~Framebuffer_OpenGL() override;

        virtual void Bind() override;
        virtual bool IsBound() const override;
    };
}

namespace Game::Graphics::Framebuffer {

    Game::Graphics::Classes::Framebuffer* (*Create)();
    void (*CreateBulk)(size_t CreateAmount,
                       Game::Graphics::Classes::Framebuffer* Textures[]);
    void (*Unbind)();

    inline void Init_OpenGL();
    inline void Init_Vulkan();
    inline void Init_Metal();
    inline void Init_DirectX11();
    inline void Init_DirectX12();
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

static Game::Graphics::Classes::Framebuffer* _Framebuffer_Create_OpenGL() {
    Game::Graphics::Classes::Framebuffer_OpenGL* NewFramebuffer =
        new Game::Graphics::Classes::Framebuffer_OpenGL;

    Game::Graphics::OpenGLFunctions::glGenFramebuffers(
        1, &NewFramebuffer->GLObject);

    return reinterpret_cast<Game::Graphics::Classes::Framebuffer*>(
        NewFramebuffer);
}

static void _Framebuffer_CreateBulk_OpenGL(
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

static void _Framebuffer_Unbind_OpenGL() {
    Game::Graphics::OpenGLFunctions::glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Game::Graphics::Framebuffer::Init_OpenGL() {
    LOAD_OPENGL_FUNCTION(glGenFramebuffers);
    LOAD_OPENGL_FUNCTION(glDeleteFramebuffers);

    LOAD_OPENGL_FUNCTION(glBindFramebuffer);

    Framebuffer::Create = &_Framebuffer_Create_OpenGL;
    Framebuffer::CreateBulk = &_Framebuffer_CreateBulk_OpenGL;

    Framebuffer::Unbind = &_Framebuffer_Unbind_OpenGL;
}

void Game::Graphics::Framebuffer::Init_Vulkan() {}

void Game::Graphics::Framebuffer::Init_Metal() {}

void Game::Graphics::Framebuffer::Init_DirectX11() {}

void Game::Graphics::Framebuffer::Init_DirectX12() {}

#endif