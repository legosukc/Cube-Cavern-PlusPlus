#pragma once

#include "../../define.h"

#include <cstddef>

#include <SDL3/SDL_opengl.h>
#include <SDL3/SDL_opengl_glext.h>

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