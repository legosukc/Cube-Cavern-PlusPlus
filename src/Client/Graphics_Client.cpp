#include "../define.h"

#include "Graphics_Client.hpp"

#include <iostream>

#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_video.h>

#include <SDL3/SDL_opengl.h>
#include <SDL3/SDL_opengl_glext.h>
#include <SDL3/SDL_opengles2_gl2ext.h>

#include "../Game.hpp"

namespace Game::Graphics {

    Uint8 AnisotropicFilteringLevel = 1;

    namespace OpenGL {
        int MajorVersion;
        int MinorVersion;
        int Profile;
    }

    GraphicsAPIEnum ActiveAPI;
}

static void (*_SetClearColor)(float R, float G, float B, float A);
static void (*_ClearBitfields)(unsigned int Bitfields);

void Game::Graphics::SetClearColor(float R, float G, float B, float A) {
    _SetClearColor(R, G, B, A);
}

void Game::Graphics::ClearBitfields(unsigned int Bitfields) {
    _ClearBitfields(Bitfields);
}

constexpr _Enums_BindTargets::_Enums_BindTargets() = default;
constexpr _Enums_BindTargets::_Enums_BindTargets(IntType _VertexBuffer,
                                                 IntType _ElementBuffer,
                                                 IntType _UniformBuffer)
    : VertexBuffer(_VertexBuffer),
      ElementBuffer(_ElementBuffer),
      UniformBuffer(_UniformBuffer) {}

constexpr _Enums_AllocationTypes::_Enums_AllocationTypes() = default;
constexpr _Enums_AllocationTypes::_Enums_AllocationTypes(IntType _Streaming,
                                                         IntType _Drawing,
                                                         IntType _Reading)
    : Streaming(_Streaming), Drawing(_Drawing), Reading(_Reading) {}

constexpr _Enums_BufferBitfields::_Enums_BufferBitfields() = default;
constexpr _Enums_BufferBitfields::_Enums_BufferBitfields(IntType _ColorBit,
                                                         IntType _DepthBit,
                                                         IntType _StencilBit)
    : ColorBit(_ColorBit), DepthBit(_DepthBit), StencilBit(_StencilBit) {}

void _Namespace_Buffer::CopyToBuffer(
    _Enums_BindTargets::IntType BindTarget,
    size_t Size,
    const void* Data,
    _Enums_AllocationTypes::IntType AllocateFor) {}

void _Namespace_Buffer::CopyToBufferPointer(
    _Enums_BindTargets::IntType BindTarget,
    int Offset,
    size_t Size,
    const void* Data) {}

void _Namespace_Buffer::AllocateBuffer(
    _Enums_BindTargets::IntType BindTarget,
    size_t Size,
    _Enums_AllocationTypes::IntType AllocateFor) {}

namespace Game::Graphics {
    ::_Enums_BindTargets BindTargets;
    ::_Enums_AllocationTypes AllocationTypes;
    ::_Enums_BufferBitfields BufferBitfields;
}

namespace Game::Graphics::OpenGLFunctions {
    PFNGLCLEARCOLORPROC glClearColor;
    PFNGLCLEARPROC glClear;

    PFNGLGETINTEGERVPROC glGetIntegerv;
    PFNGLISENABLEDPROC glIsEnabled;

    PFNGLDRAWARRAYSPROC glDrawArrays;
    PFNGLDRAWARRAYSINSTANCEDPROC glDrawArraysInstanced;

    PFNGLDRAWELEMENTSPROC glDrawElements;
    PFNGLDRAWELEMENTSINSTANCEDPROC glDrawElementsInstanced;
}

static void _SetClearColor_OpenGL(float R, float G, float B, float A) {
    Game::Graphics::OpenGLFunctions::glClearColor(R, G, B, A);
}

static void _ClearBitfields_OpenGL(unsigned int Bitfields) {
    Game::Graphics::OpenGLFunctions::glClear(Bitfields);
}

#include "GraphicsClasses/BufferBase.hpp"

#include "GraphicsClasses/IndexBuffer.hpp"
#include "GraphicsClasses/VertexArray.hpp"
#include "GraphicsClasses/VertexBuffer.hpp"

#include "GraphicsClasses/UniformBuffer.hpp"

#include "GraphicsClasses/FragmentShader.hpp"
#include "GraphicsClasses/ShaderBase.hpp"
#include "GraphicsClasses/VertexShader.hpp"

#include "GraphicsClasses/Program.hpp"

#include "GraphicsClasses/Framebuffer.hpp"

#include "GraphicsClasses/Texture.hpp"

namespace Game::Graphics {
    Game::Graphics::BoundObjectsStruct BoundObjects;
    Game::Graphics::GBufferStruct GBuffer;
}

void Game::Graphics::Init() {
    Graphics::ActiveAPI = Game::Graphics::GraphicsAPIEnum::OpenGL;

    switch (Graphics::ActiveAPI) {
        case Graphics::GraphicsAPIEnum::OpenGL:
        OpenGLSetup:

            SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,
                                &Graphics::OpenGL::MajorVersion);
            SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,
                                &Graphics::OpenGL::MinorVersion);
            SDL_GL_GetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                                &Graphics::OpenGL::Profile);

            if (Graphics::OpenGL::MajorVersion < 3) {
                static const char* Extensions[] = {"GL_EXT_draw_buffers"};

                // PFNGLGENVERTEXARRAYSOESPROC
                for (const char* Extension : Extensions) {
                    if (!SDL_GL_ExtensionSupported(Extension)) {
                        std::cerr << "" << std::endl;
                    }
                }
            }

            LOAD_OPENGL_FUNCTION(glClear);
            LOAD_OPENGL_FUNCTION(glClearColor);

            LOAD_OPENGL_FUNCTION(glDrawArrays);
            LOAD_OPENGL_FUNCTION(glDrawArraysInstanced);

            LOAD_OPENGL_FUNCTION(glDrawElements);
            LOAD_OPENGL_FUNCTION(glDrawElementsInstanced);

            LOAD_OPENGL_FUNCTION(glGetIntegerv);
            LOAD_OPENGL_FUNCTION(glIsEnabled);

            _SetClearColor = _SetClearColor_OpenGL;
            _ClearBitfields = _ClearBitfields_OpenGL;

            Game::Graphics::BindTargets = _Enums_BindTargets(
                GL_ARRAY_BUFFER, GL_ELEMENT_ARRAY_BUFFER, GL_UNIFORM_BUFFER);

            Game::Graphics::AllocationTypes = ::_Enums_AllocationTypes(
                GL_STREAM_DRAW, GL_STATIC_DRAW, GL_STATIC_READ);
            Game::Graphics::BufferBitfields = ::_Enums_BufferBitfields(
                GL_COLOR_BUFFER_BIT, GL_DEPTH_BUFFER_BIT,
                GL_STENCIL_BUFFER_BIT);

            Graphics::BufferBase::Init_OpenGL();

            Graphics::VertexArray::Init_OpenGL();
            Graphics::VertexBuffer::Init_OpenGL();
            Graphics::IndexBuffer::Init_OpenGL();

            Graphics::UniformBuffer::Init_OpenGL();

            Graphics::ShaderBase::Init_OpenGL();
            Graphics::VertexShader::Init_OpenGL();
            Graphics::FragmentShader::Init_OpenGL();

            Graphics::Program::Init_OpenGL();

            Graphics::Texture::Init_OpenGL();

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
        reinterpret_cast<Classes::Texture**>(&GBuffer)[i]->Bind();
        Graphics::Texture::UploadPixelData(WindowSize.X, WindowSize.Y, GL_RGB,
                                           GL_FLOAT, NULL);
        Graphics::Texture::AttachToFramebuffer(
            GL_COLOR_ATTACHMENT0 + i,
            reinterpret_cast<Classes::Texture**>(&GBuffer)[i]);
    }
}

void Game::Graphics::Update() {}

void Game::Graphics::Draw() {}