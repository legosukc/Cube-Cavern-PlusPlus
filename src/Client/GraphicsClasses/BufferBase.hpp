#pragma once

#include <cstddef>

#include "BaseClassDefinitions.hpp"

namespace Game::Graphics::OpenGLFunctions {

    extern PFNGLGENBUFFERSPROC glGenBuffers;
    extern PFNGLDELETEBUFFERSPROC glDeleteBuffers;

    extern PFNGLBUFFERDATAPROC glBufferData;
    extern PFNGLBUFFERSUBDATAPROC glBufferSubData;

    extern PFNGLBINDBUFFERPROC glBindBuffer;
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