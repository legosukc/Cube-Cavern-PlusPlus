#ifndef GRAPHICS_BUFFERBASE_H
#define GRAPHICS_BUFFERBASE_H

#include "../../define.h"

#include <cstddef>

#include <SDL3/SDL_opengl.h>
#include <SDL3/SDL_opengl_glext.h>
#include <SDL3/SDL_video.h>

#include "BaseClassDefinitions.hpp"

namespace Game::Graphics::OpenGLFunctions {
    PFNGLGENBUFFERSPROC glGenBuffers;
    PFNGLDELETEBUFFERSPROC glDeleteBuffers;

    PFNGLBUFFERDATAPROC glBufferData;
    PFNGLBUFFERSUBDATAPROC glBufferSubData;

    PFNGLBINDBUFFERPROC glBindBuffer;
}

namespace Game::Graphics::Classes {

    class BufferBase : public Game::Graphics::Classes::OpenGLClassBase {
       public:
        virtual ~BufferBase();

        virtual void Bind();
        virtual bool IsBound() const;
    };

    template <GLenum BufferTarget>
    class BufferBase_OpenGL : virtual public BufferBase {
       public:
        virtual ~BufferBase_OpenGL() override;

        virtual void Bind() override;

        constexpr static inline GLenum OpenGLBufferEnum = BufferTarget;
    };
}

namespace Game::Graphics::BufferBase {

    namespace WrapperTemplates {

        template <class BufferClass, class BufferOpenGLClass>
        BufferClass* Create_OpenGL();

        template <class BufferClass, class BufferOpenGLClass>
        void CreateBulk_OpenGL(size_t CreateAmount, BufferClass* Buffers[]);

        template <class BufferOpenGLClass>
        inline void Unbind_OpenGL();
    }

    inline void Init_OpenGL();
    inline void Init_Vulkan();
    inline void Init_Metal();
    inline void Init_DirectX11();
    inline void Init_DirectX12();
}

Game::Graphics::Classes::BufferBase::~BufferBase() = default;

void Game::Graphics::Classes::BufferBase::Bind() {}
bool Game::Graphics::Classes::BufferBase::IsBound() const {
    return false;
}

template <GLenum BufferTarget>
Game::Graphics::Classes::BufferBase_OpenGL<BufferTarget>::~BufferBase_OpenGL() {
    unlikely_branch if (this->GLObject == 0) {
#ifdef DEBUG_BUILD
        std::cerr << "Attempted to free an unallocated OpenGL buffer."
                  << std::endl;
        __debugbreak();
#endif
        return;
    }
    Game::Graphics::OpenGLFunctions::glDeleteBuffers(1, &this->GLObject);
}

template <GLenum BufferTarget>
void Game::Graphics::Classes::BufferBase_OpenGL<BufferTarget>::Bind() {
    Game::Graphics::OpenGLFunctions::glBindBuffer(BufferTarget, this->GLObject);
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
        Game::Graphics::OpenGLFunctions::glGenBuffers(1, &Buffers[i]->GLObject);
    }
}

template <class BufferOpenGLClass>
void Game::Graphics::BufferBase::WrapperTemplates::Unbind_OpenGL() {
    Game::Graphics::OpenGLFunctions::glBindBuffer(
        BufferOpenGLClass::OpenGLBufferEnum, 0);
}

void Game::Graphics::BufferBase::Init_OpenGL() {
    LOAD_OPENGL_FUNCTION(glGenBuffers);
    LOAD_OPENGL_FUNCTION(glDeleteBuffers);

    LOAD_OPENGL_FUNCTION(glBindBuffer);

    LOAD_OPENGL_FUNCTION(glBufferData);
    LOAD_OPENGL_FUNCTION(glBufferSubData);
}

void Game::Graphics::BufferBase::Init_Vulkan() {}

void Game::Graphics::BufferBase::Init_Metal() {}

void Game::Graphics::BufferBase::Init_DirectX11() {}

void Game::Graphics::BufferBase::Init_DirectX12() {}

#endif