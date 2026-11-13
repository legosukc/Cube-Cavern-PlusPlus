#pragma once

#include "../define.h"

#include <iostream>

#include <SDL3/SDL_platform_defines.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_video.h>

#include "../Game.hpp"

namespace Game::Graphics {
    inline void Init();

    enum class WindowMode : Uint8 { Windowed, Borderless, Fullscreen };

    extern Uint8 AnisotropicFilteringLevel;

    enum class GraphicsAPIEnum : Uint8 {
        None,
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

#include "GraphicsOpenGL_Client.hpp"

namespace Game::Graphics::_misc {

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

#include "GraphicsClasses/BufferBase.hpp"

#include "GraphicsClasses/IndexBuffer.hpp"
#include "GraphicsClasses/VertexArray.hpp"
#include "GraphicsClasses/VertexBuffer.hpp"

#include "GraphicsClasses/UniformBuffer.hpp"

#include "GraphicsClasses/FragmentShader.hpp"
#include "GraphicsClasses/Program.hpp"
#include "GraphicsClasses/ShaderBase.hpp"
#include "GraphicsClasses/VertexShader.hpp"


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

    extern void (*_SetClearColor)(float R, float G, float B, float A);
    extern void (*_ClearBitfields)(unsigned int Bitfields);
}

void Game::Graphics::SetClearColor(float R, float G, float B, float A) {
    _SetClearColor(R, G, B, A);
}

void Game::Graphics::ClearBitfields(unsigned int Bitfields) {
    _ClearBitfields(Bitfields);
}

constexpr Game::Graphics::_misc::_Enums_BindTargets::_Enums_BindTargets() =
    default;
constexpr Game::Graphics::_misc::_Enums_BindTargets::_Enums_BindTargets(
    IntType _VertexBuffer,
    IntType _ElementBuffer,
    IntType _UniformBuffer)
    : VertexBuffer(_VertexBuffer),
      ElementBuffer(_ElementBuffer),
      UniformBuffer(_UniformBuffer) {}

constexpr Game::Graphics::_misc::_Enums_AllocationTypes::
    _Enums_AllocationTypes() = default;
constexpr Game::Graphics::_misc::_Enums_AllocationTypes::_Enums_AllocationTypes(
    IntType _Streaming,
    IntType _Drawing,
    IntType _Reading)
    : Streaming(_Streaming), Drawing(_Drawing), Reading(_Reading) {}

constexpr Game::Graphics::_misc::_Enums_BufferBitfields::
    _Enums_BufferBitfields() = default;
constexpr Game::Graphics::_misc::_Enums_BufferBitfields::_Enums_BufferBitfields(
    IntType _ColorBit,
    IntType _DepthBit,
    IntType _StencilBit)
    : ColorBit(_ColorBit), DepthBit(_DepthBit), StencilBit(_StencilBit) {}

namespace Game::Graphics {
    extern Game::Graphics::_misc::_Enums_BindTargets BindTargets;
    extern Game::Graphics::_misc::_Enums_AllocationTypes AllocationTypes;
    extern Game::Graphics::_misc::_Enums_BufferBitfields BufferBitfields;
}

static void _SetClearColor_OpenGL(float R, float G, float B, float A) {
    Game::Graphics::OpenGLFunctions::glClearColor(R, G, B, A);
}

static void _ClearBitfields_OpenGL(unsigned int Bitfields) {
    Game::Graphics::OpenGLFunctions::glClear(Bitfields);
}

static void _graphics_WindowResizedEvent_updateGLViewport(
    const Math::IVector2& Size) {
    Game::Graphics::OpenGLFunctions::glViewport(0, 0, Size.X, Size.Y);
}

void Game::Graphics::Init() {
    // temp
    Graphics::ActiveAPI = Game::Graphics::GraphicsAPIEnum::OpenGL;

    switch (Graphics::ActiveAPI) {
        case Graphics::GraphicsAPIEnum::OpenGL:
        OpenGLSetup:

            SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,
                                &Game::Graphics::OpenGL::MajorVersion);
            SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,
                                &Game::Graphics::OpenGL::MinorVersion);
            SDL_GL_GetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                                &Game::Graphics::OpenGL::Profile);

            /*
if (Graphics::OpenGL::MajorVersion < 3) {
static const char* Extensions[] = {"GL_EXT_draw_buffers"};
for (const char* Extension : Extensions) {
if (!Game::Graphics::OpenGL::ExtensionSupported(
        Extension)) {
    std::cerr << "" << std::endl;
}
}
}*/

            LOAD_OPENGL_FUNCTION(glViewport);

            LOAD_OPENGL_FUNCTION(glEnable);
            LOAD_OPENGL_FUNCTION(glDisable);

            LOAD_OPENGL_FUNCTION(glClear);
            LOAD_OPENGL_FUNCTION(glClearColor);

            LOAD_OPENGL_FUNCTION(glBlendFunc);

            LOAD_OPENGL_FUNCTION(glDrawArrays);
            LOAD_OPENGL_FUNCTION(glDrawElements);

#ifndef SDL_PLATFORM_VITA
            LOAD_OPENGL_FUNCTION(glDrawArraysInstanced);
            LOAD_OPENGL_FUNCTION(glDrawElementsInstanced);
#endif

            LOAD_OPENGL_FUNCTION(glGetIntegerv);
            LOAD_OPENGL_FUNCTION(glGetString);

#ifndef SDL_PLATFORM_VITA
            LOAD_OPENGL_FUNCTION(glGetStringi);
#endif

            LOAD_OPENGL_FUNCTION(glIsEnabled);

            Game::Graphics::OpenGLFunctions::glBlendFunc(
                GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            _SetClearColor = _SetClearColor_OpenGL;
            _ClearBitfields = _ClearBitfields_OpenGL;

#ifdef SDL_PLATFORM_VITA
            Game::Graphics::BindTargets =
                Game::Graphics::_misc::_Enums_BindTargets(
                    GL_ARRAY_BUFFER, GL_ELEMENT_ARRAY_BUFFER, -1);

            Game::Graphics::AllocationTypes =
                Game::Graphics::_misc::_Enums_AllocationTypes(
                    GL_STREAM_DRAW, GL_STATIC_DRAW, -1);
#else
            Game::Graphics::BindTargets =
                Game::Graphics::_misc::_Enums_BindTargets(
                    GL_ARRAY_BUFFER, GL_ELEMENT_ARRAY_BUFFER,
                    GL_UNIFORM_BUFFER);

            Game::Graphics::AllocationTypes =
                Game::Graphics::_misc::_Enums_AllocationTypes(
                    GL_STREAM_DRAW, GL_STATIC_DRAW, GL_STATIC_READ);
#endif

            Game::Graphics::BufferBitfields =
                Game::Graphics::_misc::_Enums_BufferBitfields(
                    GL_COLOR_BUFFER_BIT, GL_DEPTH_BUFFER_BIT,
                    GL_STENCIL_BUFFER_BIT);

            std::cout << "init buffer base" << std::endl;
            Graphics::BufferBase::Init_OpenGL();
            std::cout << "Initialized BufferBase" << std::endl;

            Graphics::VertexArray::Init_OpenGL();
            std::cout << "Initialized VertexArray" << std::endl;

            Graphics::VertexBuffer::Init_OpenGL();
            std::cout << "Initialized VertexBuffer" << std::endl;

            Graphics::IndexBuffer::Init_OpenGL();
            std::cout << "Initialized IndexBuffer" << std::endl;

            Graphics::UniformBuffer::Init_OpenGL();
            std::cout << "Initialized UniformBuffer" << std::endl;

            Graphics::ShaderBase::Init_OpenGL();
            std::cout << "Initialized ShaderBase" << std::endl;

            Graphics::VertexShader::Init_OpenGL();
            std::cout << "Initialized VertexShader" << std::endl;

            Graphics::FragmentShader::Init_OpenGL();
            std::cout << "Initialized FragmentShader" << std::endl;

            Graphics::Program::Init_OpenGL();
            std::cout << "Initialized Program" << std::endl;

            Graphics::Texture::Init_OpenGL();
            std::cout << "Initialized Texture" << std::endl;

            Graphics::Framebuffer::Init_OpenGL();
            std::cout << "Initialized Framebuffer" << std::endl;

#ifdef BUILD_CLIENT
            Game::Window.WindowResizedEvent.Connect(
                ::_graphics_WindowResizedEvent_updateGLViewport);
#endif

            Graphics::OpenGLFunctions::glEnable(GL_DEPTH_TEST);
            break;

        case GraphicsAPIEnum::Vulkan:

            Graphics::BufferBase::Init_Vulkan();

            Graphics::VertexArray::Init_Vulkan();
            Graphics::VertexBuffer::Init_Vulkan();
            Graphics::IndexBuffer::Init_Vulkan();

            Graphics::UniformBuffer::Init_Vulkan();

            Graphics::ShaderBase::Init_Vulkan();
            Graphics::VertexShader::Init_Vulkan();
            Graphics::FragmentShader::Init_Vulkan();

            Graphics::Program::Init_Vulkan();

            Graphics::Texture::Init_Vulkan();

            break;
        case GraphicsAPIEnum::Metal:

            Graphics::BufferBase::Init_Vulkan();

            Graphics::VertexArray::Init_Vulkan();
            Graphics::VertexBuffer::Init_Vulkan();
            Graphics::IndexBuffer::Init_Vulkan();

            Graphics::UniformBuffer::Init_Vulkan();

            Graphics::ShaderBase::Init_Vulkan();
            Graphics::VertexShader::Init_Vulkan();
            Graphics::FragmentShader::Init_Vulkan();

            Graphics::Program::Init_Vulkan();

            Graphics::Texture::Init_Vulkan();
            break;
        case GraphicsAPIEnum::Direct3D11:

            Graphics::BufferBase::Init_DirectX11();

            Graphics::VertexArray::Init_DirectX11();
            Graphics::VertexBuffer::Init_DirectX11();
            Graphics::IndexBuffer::Init_DirectX11();

            Graphics::UniformBuffer::Init_DirectX11();

            Graphics::ShaderBase::Init_DirectX11();
            Graphics::VertexShader::Init_DirectX11();
            Graphics::FragmentShader::Init_DirectX11();

            Graphics::Program::Init_DirectX11();

            Graphics::Texture::Init_DirectX11();
            break;

        case GraphicsAPIEnum::Direct3D12:

            Graphics::BufferBase::Init_DirectX12();

            Graphics::VertexArray::Init_DirectX12();
            Graphics::VertexBuffer::Init_DirectX12();
            Graphics::IndexBuffer::Init_DirectX12();

            Graphics::UniformBuffer::Init_DirectX12();

            Graphics::ShaderBase::Init_DirectX12();
            Graphics::VertexShader::Init_DirectX12();
            Graphics::FragmentShader::Init_DirectX12();

            Graphics::Program::Init_DirectX12();

            Graphics::Texture::Init_DirectX12();
            break;
        default:
            std::cerr << "Invalid Graphics API chosen! Using default 'OpenGL'."
                      << std::endl;
            Graphics::ActiveAPI = Game::Graphics::GraphicsAPIEnum::OpenGL;
            goto OpenGLSetup;
    };

    using Game::Graphics::GBuffer;
    Graphics::Texture::CreateBulk(
        4, reinterpret_cast<Classes::Texture**>(&GBuffer));

    const Math::IVector2 WindowSize = Game::Window.GetSize();

    for (int i = 0; i < 3; ++i) {
        reinterpret_cast<Classes::Texture**>(&GBuffer)[i]->Bind(0);
        Graphics::Texture::UploadPixelData(WindowSize.X, WindowSize.Y, GL_RGB,
                                           GL_FLOAT, NULL);
        Graphics::Texture::AttachToFramebuffer(
            GL_COLOR_ATTACHMENT0 + i,
            reinterpret_cast<Classes::Texture**>(&GBuffer)[i]);
    }
}

void Game::Graphics::Update() {}