#pragma once

#include "../define.h"

#include <iostream>

#include <SDL3/SDL_platform_defines.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_video.h>

#include "../Game.hpp"

#include "Graphics/APIHandlers/GraphicsOpenGL_Client.hpp"

#include "Graphics/Classes/BufferBase/Base_BufferBase.hpp"

#include "Graphics/Classes/IndexBuffer/Base_IndexBuffer.hpp"
#include "Graphics/Classes/VertexArray/Base_VertexArray.hpp"
#include "Graphics/Classes/VertexBuffer/Base_VertexBuffer.hpp"

#include "Graphics/Classes/UniformBuffer/Base_UniformBuffer.hpp"

#include "Graphics/Classes/FragmentShader/Base_FragmentShader.hpp"
#include "Graphics/Classes/Program/Base_Program.hpp"
#include "Graphics/Classes/ShaderBase/Base_ShaderBase.hpp"
#include "Graphics/Classes/VertexShader/Base_VertexShader.hpp"

#include "Graphics/Classes/Framebuffer/Base_Framebuffer.hpp"
#include "Graphics/Classes/Texture/Base_Texture.hpp"

#include "Graphics/Classes/Texture.hpp"

namespace Game::Graphics {
    namespace FuncPtrs {
        extern void (*ClearColorBuffer)(float R, float G, float B, float A);
        extern void (*ClearDepthBuffer)();
        extern void (*ClearStencilBuffer)();
        extern void (*ClearAllBuffers)(float R, float G, float B, float A);
    }

    inline void Init();

    enum class WindowMode : Uint8 { Windowed, Borderless, Fullscreen };

    extern Uint8 AnisotropicFilteringLevel;

    enum class GraphicsAPIEnum : Uint8 {
        None,
        Software,
        OpenGL,
        Vulkan,
        Metal,
        Direct3D11,
        Direct3D12,
        GXM,
        citro3d
    };

    extern GraphicsAPIEnum ActiveAPI;

    inline void ClearColorBuffer(float R = 0.f,
                                 float G = 0.f,
                                 float B = 0.f,
                                 float A = 1.f) {
        FuncPtrs::ClearColorBuffer(R, G, B, A);
    }
    inline void ClearDepthBuffer() {
        FuncPtrs::ClearDepthBuffer();
    }
    inline void ClearStencilBuffer() {
        FuncPtrs::ClearStencilBuffer();
    }
    inline void ClearAllBuffers(float R = 0.f,
                                float G = 0.f,
                                float B = 0.f,
                                float A = 1.f) {
        FuncPtrs::ClearAllBuffers(R, G, B, A);
    }

    inline void Update();
    void Draw();
}

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

void Game::Graphics::Init() {
    // temp
    Graphics::ActiveAPI = Game::Graphics::GraphicsAPIEnum::OpenGL;

    switch (Graphics::ActiveAPI) {
        case Graphics::GraphicsAPIEnum::OpenGL:
        OpenGLSetup:
            Game::Graphics::OpenGL::Init();
            break;

        case GraphicsAPIEnum::Vulkan:
            break;

        case GraphicsAPIEnum::Metal:
            break;

        case GraphicsAPIEnum::Direct3D11:
            break;

        case GraphicsAPIEnum::Direct3D12:
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