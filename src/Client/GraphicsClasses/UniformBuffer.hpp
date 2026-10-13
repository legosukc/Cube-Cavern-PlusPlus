#ifndef GRAPHICS_UNIFORMBUFFER_H
#define GRAPHICS_UNIFORMBUFFER_H

// TODO: make abstraction so uniform buffers work in gles 2.0

#include "../../define.h"

#include "BufferBase.hpp"

#include <SDL3/SDL_platform_defines.h>
#ifdef SDL_PLATFORM_VITA
#include <GLES2/gl2.h>
#else
#include <SDL3/SDL_opengl.h>
#include <SDL3/SDL_opengl_glext.h>
#endif

namespace Game::Graphics::OpenGLFunctions {
#ifndef SDL_PLATFORM_VITA
    inline PFNGLBINDBUFFERBASEPROC glBindBufferBase = NULL;
    inline PFNGLBINDBUFFERRANGEPROC glBindBufferRange = NULL;
#endif
}

namespace Game::Graphics::Classes {

    class UniformBuffer : virtual public Game::Graphics::Classes::BufferBase {
       protected:
        UniformBuffer();
        friend Game::Graphics::Classes::BufferBase;

       public:
        virtual void BindBase(GLuint Index);
    };

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

namespace Game::Graphics::UniformBuffer {

    inline Game::Graphics::Classes::UniformBuffer* (*Create)() = NULL;
    inline void (*CreateBulk)(
        size_t CreateAmount,
        Game::Graphics::Classes::UniformBuffer* UniformBuffers[]) = NULL;
    inline void (*Unbind)() = NULL;

    inline void Init_OpenGL();
    inline void Init_Vulkan();
    inline void Init_Metal();
    inline void Init_DirectX11();
    inline void Init_DirectX12();
}

Game::Graphics::Classes::UniformBuffer::UniformBuffer() = default;
void Game::Graphics::Classes::UniformBuffer::BindBase(GLuint Index) {}

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

void Game::Graphics::UniformBuffer::Init_OpenGL() {
#ifndef SDL_PLATFORM_VITA
    LOAD_OPENGL_FUNCTION(glBindBufferBase);
    LOAD_OPENGL_FUNCTION(glBindBufferRange);

    using namespace Game::Graphics::BufferBase;
    using UniformBuffer = Game::Graphics::Classes::UniformBuffer;
    using UniformBuffer_OpenGL = Game::Graphics::Classes::UniformBuffer_OpenGL;

    Game::Graphics::UniformBuffer::Create =
        WrapperTemplates::Create_OpenGL<UniformBuffer, UniformBuffer_OpenGL>;
    Game::Graphics::UniformBuffer::CreateBulk =
        WrapperTemplates::CreateBulk_OpenGL<UniformBuffer,
                                            UniformBuffer_OpenGL>;

    Game::Graphics::UniformBuffer::Unbind =
        WrapperTemplates::Unbind_OpenGL<UniformBuffer_OpenGL>;
#endif
}

void Game::Graphics::UniformBuffer::Init_Vulkan() {}

void Game::Graphics::UniformBuffer::Init_Metal() {}

void Game::Graphics::UniformBuffer::Init_DirectX11() {}

void Game::Graphics::UniformBuffer::Init_DirectX12() {}

#endif