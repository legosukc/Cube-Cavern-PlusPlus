#pragma once

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

#include "BaseClassDefinitions.hpp"

namespace Game::Graphics::OpenGLFunctions {
    extern PFNGLGENFRAMEBUFFERSPROC glGenFramebuffers;
    extern PFNGLDELETEFRAMEBUFFERSPROC glDeleteFramebuffers;

    extern PFNGLBINDFRAMEBUFFERPROC glBindFramebuffer;
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

    extern Game::Graphics::Classes::Framebuffer* (*Create)();
    extern void (*CreateBulk)(size_t CreateAmount,
                              Game::Graphics::Classes::Framebuffer* Textures[]);
    extern void (*Unbind)();

    inline void Init_OpenGL();
    inline void Init_Vulkan();
    inline void Init_Metal();
    inline void Init_DirectX11();
    inline void Init_DirectX12();
}

namespace Game::Graphics::Framebuffer::etc {

    Game::Graphics::Classes::Framebuffer* _Framebuffer_Create_OpenGL();

    void _Framebuffer_CreateBulk_OpenGL(
        size_t CreateAmount,
        Game::Graphics::Classes::Framebuffer* Textures[]);

    void _Framebuffer_Unbind_OpenGL();
}

void Game::Graphics::Framebuffer::Init_OpenGL() {
    LOAD_OPENGL_FUNCTION(glGenFramebuffers);
    LOAD_OPENGL_FUNCTION(glDeleteFramebuffers);

    LOAD_OPENGL_FUNCTION(glBindFramebuffer);

    Framebuffer::Create = &etc::_Framebuffer_Create_OpenGL;
    Framebuffer::CreateBulk = &etc::_Framebuffer_CreateBulk_OpenGL;

    Framebuffer::Unbind = &etc::_Framebuffer_Unbind_OpenGL;
}

void Game::Graphics::Framebuffer::Init_Vulkan() {}

void Game::Graphics::Framebuffer::Init_Metal() {}

void Game::Graphics::Framebuffer::Init_DirectX11() {}

void Game::Graphics::Framebuffer::Init_DirectX12() {}