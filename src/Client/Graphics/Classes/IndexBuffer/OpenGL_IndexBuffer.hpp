#pragma once

#include <SDL3/SDL_platform_defines.h>

#ifdef SDL_PLATFORM_VITA
#include <GLES2/gl2.h>
#include "../../../GLES2_typedefs.h"
#else
#include <SDL3/SDL_opengl.h>
#endif

#include "../BufferBase/OpenGL_BufferBase.hpp"

#include "../IndexBuffer.hpp"

namespace Game::Graphics::Classes {
    class IndexBuffer_OpenGL
        : public Game::Graphics::Classes::BufferBase_OpenGL<
              GL_ELEMENT_ARRAY_BUFFER> {
       public:
        virtual bool IsBound() const override;
    };
}

namespace Game::Graphics::IndexBuffer::OpenGL {
    inline void Init() {
        using IndexBuffer = Game::Graphics::Classes::IndexBuffer;
        using IndexBuffer_OpenGL = Game::Graphics::Classes::IndexBuffer_OpenGL;

        Game::Graphics::IndexBuffer::Create =
            Game::Graphics::BufferBase::OpenGL::Wrappers::Create<
                IndexBuffer, IndexBuffer_OpenGL>;
        Game::Graphics::IndexBuffer::CreateBulk =
            Game::Graphics::BufferBase::OpenGL::Wrappers::CreateBulk<
                IndexBuffer, IndexBuffer_OpenGL>;

        Game::Graphics::IndexBuffer::Unbind =
            Game::Graphics::BufferBase::OpenGL::Wrappers::Unbind<
                IndexBuffer_OpenGL>;
    }
}