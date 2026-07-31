#pragma once

#include "../../define.h"

#include "BufferBase.hpp"

#include <SDL3/SDL_opengl.h>
#include <SDL3/SDL_opengl_glext.h>

namespace Game::Graphics::OpenGLFunctions {
    extern PFNGLBINDBUFFERBASEPROC glBindBufferBase;
    extern PFNGLBINDBUFFERRANGEPROC glBindBufferRange;
}

namespace Game::Graphics::Classes {

    class UniformBuffer : virtual public Game::Graphics::Classes::BufferBase {
       protected:
        UniformBuffer();
        friend Game::Graphics::Classes::BufferBase;

       public:
        virtual void BindBase(GLuint Index);
    };

    class UniformBuffer_OpenGL
        : public UniformBuffer,
          public Game::Graphics::Classes::BufferBase_OpenGL<GL_UNIFORM_BUFFER> {
       public:
        virtual void BindBase(GLuint Index) override;

        virtual bool IsBound() const override;
    };
}

namespace Game::Graphics::UniformBuffer {

    extern Game::Graphics::Classes::UniformBuffer* (*Create)();
    extern void (*CreateBulk)(
        size_t CreateAmount,
        Game::Graphics::Classes::UniformBuffer* UniformBuffers[]);
    extern void (*Unbind)();

    inline void Init_OpenGL();
    inline void Init_Vulkan();
    inline void Init_Metal();
    inline void Init_DirectX11();
    inline void Init_DirectX12();
}