#pragma once

#include "../../define.h"

#include <cstddef>

#include <SDL3/SDL_opengl.h>
#include <SDL3/SDL_opengl_glext.h>

#include "BaseClassDefinitions.hpp"

namespace Game::Graphics::OpenGLFunctions {
    extern PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
    extern PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays;

    extern PFNGLBINDVERTEXARRAYPROC glBindVertexArray;

    extern PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
    extern PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray;

    extern PFNGLVERTEXATTRIBPOINTERARBPROC glVertexAttribPointer;
}

namespace Game::Graphics::Classes {

    class VertexArray : public Game::Graphics::Classes::OpenGLClassBase {
       public:
        virtual ~VertexArray();

        virtual void Bind();
        virtual bool IsBound() const;
    };

    class VertexArray_OpenGL : public VertexArray {
       public:
        virtual ~VertexArray_OpenGL() override;

        virtual void Bind() override;
        virtual bool IsBound() const override;
    };
}

namespace Game::Graphics::VertexArray {

    extern Game::Graphics::Classes::VertexArray* (*Create)();
    extern void (*CreateBulk)(size_t CreateAmount,
                       Game::Graphics::Classes::VertexArray* VertexArrays[]);
    extern void (*Unbind)();

    inline void Init_OpenGL();
    inline void Init_Vulkan();
    inline void Init_Metal();
    inline void Init_DirectX11();
    inline void Init_DirectX12();
}