#pragma once

#ifndef GRAPHICS_CLIENT_H
#define GRAPHICS_CLIENT_H

#include "../define.h"

#include <SDL3/SDL_video.h>

#include <SDL3/SDL_opengl.h>
#include <SDL3/SDL_opengl_glext.h>
#include <SDL3/SDL_opengles2_gl2ext.h>

namespace Game::Graphics {
    inline void Init();

    enum class WindowMode : Uint8 { Windowed, Borderless, Fullscreen };

    extern Uint8 AnisotropicFilteringLevel;

    namespace OpenGL {
        extern int MajorVersion;
        extern int MinorVersion;
        extern int Profile;
    }

    enum class GraphicsAPIEnum : Uint8 {
        OpenGL,
        Vulkan,
        Metal,
		Direct3D11,
        Direct3D12
    };

    extern GraphicsAPIEnum ActiveAPI;

    inline void SetClearColor(float R, float G, float B, float A);
    inline void ClearBitfields(unsigned int Bitfields);

    inline void Update();
    void Draw();
}

namespace {

    struct _Enums_BindTargets {
        constexpr _Enums_BindTargets();

        using IntType = unsigned int;

        IntType VertexBuffer{}, ElementBuffer{}, UniformBuffer{};

       private:
        constexpr _Enums_BindTargets(IntType _VertexBuffer,
                                     IntType _ElementBuffer,
                                     IntType _UniformBuffer);

        friend inline void ::Game::Graphics::Init();
    };

    struct _Enums_AllocationTypes {
        constexpr _Enums_AllocationTypes();

        using IntType = unsigned int;

        IntType Streaming{}, Drawing{}, Reading{};

       private:
        constexpr _Enums_AllocationTypes(IntType _Streaming,
                                         IntType _Drawing,
                                         IntType _Reading);

        friend inline void ::Game::Graphics::Init();
    };

    struct _Enums_BufferBitfields {
        constexpr _Enums_BufferBitfields();

        using IntType = unsigned int;

        IntType ColorBit{}, DepthBit{}, StencilBit{};

       private:
        constexpr _Enums_BufferBitfields(IntType _ColorBit,
                                         IntType _DepthBit,
                                         IntType _StencilBit);

        friend inline void ::Game::Graphics::Init();
    };

    struct _Namespace_Buffer {
        virtual void CopyToBuffer(_Enums_BindTargets::IntType BindTarget,
                                  size_t Size,
                                  const void* Data,
                                  _Enums_AllocationTypes::IntType AllocateFor);

        virtual void CopyToBufferPointer(_Enums_BindTargets::IntType BindTarget,
                                         int Offset,
                                         size_t Size,
                                         const void* Data);
        virtual void AllocateBuffer(
            _Enums_BindTargets::IntType BindTarget,
            size_t Size,
            _Enums_AllocationTypes::IntType AllocateFor);
    };
}

namespace Game::Graphics {
    extern ::_Enums_BindTargets BindTargets;
    extern ::_Enums_AllocationTypes AllocationTypes;
    extern ::_Enums_BufferBitfields BufferBitfields;
}

namespace Game::Graphics::OpenGLFunctions {
    extern PFNGLCLEARCOLORPROC glClearColor;
    extern PFNGLCLEARPROC glClear;

    extern PFNGLGETINTEGERVPROC glGetIntegerv;
    extern PFNGLISENABLEDPROC glIsEnabled;

    extern PFNGLDRAWARRAYSPROC glDrawArrays;
    extern PFNGLDRAWARRAYSINSTANCEDPROC glDrawArraysInstanced;

    extern PFNGLDRAWELEMENTSPROC glDrawElements;
    extern PFNGLDRAWELEMENTSINSTANCEDPROC glDrawElementsInstanced;
}

#include "GraphicsClasses/IndexBuffer.hpp"
#include "GraphicsClasses/VertexArray.hpp"
#include "GraphicsClasses/VertexBuffer.hpp"

#include "GraphicsClasses/UniformBuffer.hpp"

#include "GraphicsClasses/Program.hpp"

#include "GraphicsClasses/Framebuffer.hpp"
#include "GraphicsClasses/Texture.hpp"

namespace Game::Graphics {

    struct BoundObjectsStruct {
        Classes::VertexArray* VertexArray;
        Classes::VertexBuffer* VertexBuffer;
        Classes::IndexBuffer* IndexBuffer;
        Classes::UniformBuffer* UniformBuffer;

        Classes::Program* Program;
        Classes::Texture* Texture;
    };
    extern BoundObjectsStruct BoundObjects;

    struct GBufferStruct {
        Classes::Texture *Position, *Normal, *Albedo, *Specular;
    };
    extern GBufferStruct GBuffer;
}

#endif