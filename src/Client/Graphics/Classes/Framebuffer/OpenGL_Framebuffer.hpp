#pragma once

#include "../../../../define.h"

#ifdef SDL_PLATFORM_VITA
#include <GLES2/gl2.h>
#include "../../../../GLES2_typedefs.h"
#else
#include <SDL3/SDL_opengl.h>
#include <SDL3/SDL_opengl_glext.h>
#endif

#include "Base_Framebuffer.hpp"

#include "../Framebuffer.hpp"

namespace Game::Graphics::OpenGLFunctions {
    extern PFNGLGENFRAMEBUFFERSPROC glGenFramebuffers;
    extern PFNGLDELETEFRAMEBUFFERSPROC glDeleteFramebuffers;

    extern PFNGLBINDFRAMEBUFFERPROC glBindFramebuffer;
}

namespace Game::Graphics::Classes {

    class Framebuffer_OpenGL : public Framebuffer {
       public:
        virtual ~Framebuffer_OpenGL() override;

        virtual void Bind() override;
        virtual bool IsBound() const override;
    };
}

namespace Game::Graphics::Framebuffer::OpenGL::Wrappers {

    Game::Graphics::Classes::Framebuffer* Create();
    void CreateBulk(size_t CreateAmount,
                    Game::Graphics::Classes::Framebuffer* Framebuffers[]);
    void Unbind();
}

namespace Game::Graphics::Framebuffer::OpenGL {
    inline void Init() {
        LOAD_OPENGL_FUNCTION(glGenFramebuffers);
        LOAD_OPENGL_FUNCTION(glDeleteFramebuffers);

        LOAD_OPENGL_FUNCTION(glBindFramebuffer);

        Framebuffer::Create = &Framebuffer::OpenGL::Wrappers::Create;
        Framebuffer::CreateBulk =
            &Framebuffer::OpenGL::Wrappers::CreateBulk;

        Framebuffer::Unbind = &Framebuffer::OpenGL::Wrappers::Unbind;
    }
}