#pragma once

#ifdef SDL_PLATFORM_VITA
#include <GLES2/gl2.h>
#include "../../../../GLES2_typedefs.h"
#else
#include <SDL3/SDL_opengl.h>
#include <SDL3/SDL_opengl_glext.h>
#endif

#include "../BufferBase/OpenGL_BufferBase.hpp"
#include "Base_UniformBuffer.hpp"

#include "../UniformBuffer.hpp"

namespace Game::Graphics::OpenGLFunctions {
#ifndef SDL_PLATFORM_VITA
    extern PFNGLBINDBUFFERBASEPROC glBindBufferBase;
    extern PFNGLBINDBUFFERRANGEPROC glBindBufferRange;
#endif
}

namespace Game::Graphics::Classes {

#ifndef SDL_PLATFORM_VITA
    class UniformBuffer_OpenGL
        : public UniformBuffer,
          public Game::Graphics::Classes::BufferBase_OpenGL<GL_UNIFORM_BUFFER> {
       public:
        virtual void BindBase(GLuint Index) override;

        virtual bool IsBound() const override;
    };
#endif
}

namespace Game::Graphics::UniformBuffer::OpenGL {
    inline void Init() {
#ifndef SDL_PLATFORM_VITA
        LOAD_OPENGL_FUNCTION(glBindBufferBase);
        LOAD_OPENGL_FUNCTION(glBindBufferRange);

        using UniformBuffer = Game::Graphics::Classes::UniformBuffer;
        using UniformBuffer_OpenGL =
            Game::Graphics::Classes::UniformBuffer_OpenGL;

        Game::Graphics::UniformBuffer::Create =
            BufferBase::OpenGL::Wrappers::Create<UniformBuffer,
                                                 UniformBuffer_OpenGL>;
        Game::Graphics::UniformBuffer::CreateBulk =
            BufferBase::OpenGL::Wrappers::CreateBulk<UniformBuffer,
                                                     UniformBuffer_OpenGL>;

        Game::Graphics::UniformBuffer::Unbind =
            BufferBase::OpenGL::Wrappers::Unbind<UniformBuffer_OpenGL>;
#endif
    }
}