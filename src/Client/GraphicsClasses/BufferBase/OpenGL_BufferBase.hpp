#pragma once

#include "../../../define.h"

#include "Base_BufferBase.hpp"

#include "../BaseClassDefinitions.hpp"

#include <SDL3/SDL_platform_defines.h>

#ifdef SDL_PLATFORM_VITA
#include <GLES2/gl2.h>
#include "../../GLES2_typedefs.h"
#else
#include <SDL3/SDL_opengl.h>
#include <SDL3/SDL_opengl_glext.h>
#endif

namespace Game::Graphics::OpenGLFunctions {
    extern PFNGLGENBUFFERSPROC glGenBuffers;
    extern PFNGLDELETEBUFFERSPROC glDeleteBuffers;

    extern PFNGLBUFFERDATAPROC glBufferData;
    extern PFNGLBUFFERSUBDATAPROC glBufferSubData;

    extern PFNGLBINDBUFFERPROC glBindBuffer;
}

namespace Game::Graphics::Classes {
    template <GLenum BufferTarget>
    class BufferBase_OpenGL : public BufferBase, public OpenGLClassBase {
       public:
        virtual ~BufferBase_OpenGL() override;

        virtual void Bind() override;

        constexpr static GLenum OpenGLBufferEnum = BufferTarget;
    };
}

namespace Game::Graphics::BufferBase::WrapperTemplates {

    template <class BufferClass, class BufferOpenGLClass>
    BufferClass* Create_OpenGL();

    template <class BufferClass, class BufferOpenGLClass>
    void CreateBulk_OpenGL(size_t CreateAmount, BufferClass* Buffers[]);

    template <class BufferOpenGLClass>
    void Unbind_OpenGL();
}

template <class BufferClass, class BufferOpenGLClass>
BufferClass* Game::Graphics::BufferBase::WrapperTemplates::Create_OpenGL() {
    BufferOpenGLClass* NewBuffer = new BufferOpenGLClass;
    Game::Graphics::OpenGLFunctions::glGenBuffers(1, &NewBuffer->GLObject);

    return reinterpret_cast<BufferClass*>(NewBuffer);
}

template <class BufferClass, class BufferOpenGLClass>
void Game::Graphics::BufferBase::WrapperTemplates::CreateBulk_OpenGL(
    size_t CreateAmount,
    BufferClass* Buffers[]) {
    for (size_t i = 0; i < CreateAmount; ++i) {
        Buffers[i] = reinterpret_cast<BufferClass*>(new BufferOpenGLClass());
        Game::Graphics::OpenGLFunctions::glGenBuffers(
            1, &reinterpret_cast<BufferOpenGLClass*>(Buffers)[i].GLObject);
    }
}

template <class BufferOpenGLClass>
void Game::Graphics::BufferBase::WrapperTemplates::Unbind_OpenGL() {
    Game::Graphics::OpenGLFunctions::glBindBuffer(
        BufferOpenGLClass::OpenGLBufferEnum, 0);
}