#pragma once

#include "../../../define.h"

#include <SDL3/SDL.h>

#ifdef SDL_PLATFORM_VITA
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include "../../../GLES2_typedefs.h"
#else
#include <SDL3/SDL_opengl.h>
#include <SDL3/SDL_opengl_glext.h>
#include <SDL3/SDL_opengles2_gl2ext.h>
#endif

#include "../../../Game.hpp"

#include "../Classes/GraphicsEnums.hpp"

#include "../Classes/BufferBase/OpenGL_BufferBase.hpp"

#include "../Classes/IndexBuffer/OpenGL_IndexBuffer.hpp"
#include "../Classes/VertexArray/OpenGL_VertexArray.hpp"
#include "../Classes/VertexBuffer/OpenGL_VertexBuffer.hpp"

#include "../Classes/UniformBuffer/OpenGL_UniformBuffer.hpp"

#include "../Classes/FragmentShader/OpenGL_FragmentShader.hpp"
#include "../Classes/Program/OpenGL_Program.hpp"
#include "../Classes/ShaderBase/OpenGL_ShaderBase.hpp"
#include "../Classes/VertexShader/OpenGL_VertexShader.hpp"

#include "../Classes/Framebuffer/OpenGL_Framebuffer.hpp"
#include "../Classes/Texture/OpenGL_Texture.hpp"

namespace Game::Graphics::OpenGLFunctions {

    extern PFNGLVIEWPORTPROC glViewport;

    extern PFNGLENABLEPROC glEnable;
    extern PFNGLDISABLEPROC glDisable;

    extern PFNGLCLEARCOLORPROC glClearColor;
    extern PFNGLCLEARPROC glClear;

    extern PFNGLBLENDFUNCPROC glBlendFunc;

    extern PFNGLGETINTEGERVPROC glGetIntegerv;
    extern PFNGLGETSTRINGPROC glGetString;

#ifndef SDL_PLATFORM_VITA
    extern PFNGLGETSTRINGIPROC glGetStringi;
#endif

    extern PFNGLISENABLEDPROC glIsEnabled;

    extern PFNGLDRAWARRAYSPROC glDrawArrays;
    extern PFNGLDRAWELEMENTSPROC glDrawElements;

#ifndef SDL_PLATFORM_VITA
    extern PFNGLDRAWARRAYSINSTANCEDPROC glDrawArraysInstanced;
    extern PFNGLDRAWELEMENTSINSTANCEDPROC glDrawElementsInstanced;
#endif
}

// from Graphics_Client
namespace Game::Graphics::FuncPtrs {
    extern void (*ClearColorBuffer)(float R , float G, float B, float A);
    extern void (*ClearDepthBuffer)();
    extern void (*ClearStencilBuffer)();
    extern void (*ClearAllBuffers)(float R, float G, float B, float A);
}

namespace Game::Graphics::OpenGL {

    namespace Wrappers {
        void ClearColorBuffer(float R, float G, float B, float A);
        void ClearDepthBuffer();
        void ClearStencilBuffer();
        void ClearAllBuffers(float R, float G, float B, float A);
        void WindowResizedEvent_UpdateGLViewport(const Math::IVector2& Size);
    }

    extern int MajorVersion;
    extern int MinorVersion;
    extern int Profile;

    inline void Init();
}

void Game::Graphics::OpenGL::Init() {
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,
                        &Game::Graphics::OpenGL::MajorVersion);
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,
                        &Game::Graphics::OpenGL::MinorVersion);
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                        &Game::Graphics::OpenGL::Profile);

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

    Game::Graphics::OpenGLFunctions::glBlendFunc(GL_SRC_ALPHA,
                                                 GL_ONE_MINUS_SRC_ALPHA);

    Game::Graphics::FuncPtrs::ClearColorBuffer = Wrappers::ClearColorBuffer;
    Game::Graphics::FuncPtrs::ClearDepthBuffer = Wrappers::ClearDepthBuffer;
    Game::Graphics::FuncPtrs::ClearStencilBuffer = Wrappers::ClearStencilBuffer;
    Game::Graphics::FuncPtrs::ClearAllBuffers = Wrappers::ClearAllBuffers;

#ifdef SDL_PLATFORM_VITA
    Game::Graphics::BindTargets = Game::Graphics::_misc::_Enums_BindTargets(
        GL_ARRAY_BUFFER, GL_ELEMENT_ARRAY_BUFFER, -1);

    Game::Graphics::AllocationTypes =
        Game::Graphics::_misc::_Enums_AllocationTypes(GL_STREAM_DRAW,
                                                      GL_STATIC_DRAW, -1);
#else
    Game::Graphics::BindTargets = Game::Graphics::_misc::_Enums_BindTargets(
        GL_ARRAY_BUFFER, GL_ELEMENT_ARRAY_BUFFER, GL_UNIFORM_BUFFER);

    Game::Graphics::AllocationTypes =
        Game::Graphics::_misc::_Enums_AllocationTypes(
            GL_STREAM_DRAW, GL_STATIC_DRAW, GL_STATIC_READ);
#endif

    Graphics::BufferBase::OpenGL::Init();
    std::cout << "Initialized BufferBase" << std::endl;

    Graphics::VertexArray::OpenGL::Init();
    std::cout << "Initialized VertexArray" << std::endl;

    Graphics::VertexBuffer::OpenGL::Init();
    std::cout << "Initialized VertexBuffer" << std::endl;

    Graphics::IndexBuffer::OpenGL::Init();
    std::cout << "Initialized IndexBuffer" << std::endl;

    Graphics::UniformBuffer::OpenGL::Init();
    std::cout << "Initialized UniformBuffer" << std::endl;

    Graphics::ShaderBase::OpenGL::Init();
    std::cout << "Initialized ShaderBase" << std::endl;

    Graphics::VertexShader::OpenGL::Init();
    std::cout << "Initialized VertexShader" << std::endl;

    Graphics::FragmentShader::OpenGL::Init();
    std::cout << "Initialized FragmentShader" << std::endl;

    Graphics::Program::OpenGL::Init();
    std::cout << "Initialized Program" << std::endl;

    Graphics::Texture::OpenGL::Init();
    std::cout << "Initialized Texture" << std::endl;

    Graphics::Framebuffer::OpenGL::Init();
    std::cout << "Initialized Framebuffer" << std::endl;

#ifdef BUILD_CLIENT
    Game::Window.WindowResizedEvent.Connect(
        Wrappers::WindowResizedEvent_UpdateGLViewport);
#endif

    Graphics::OpenGLFunctions::glEnable(GL_DEPTH_TEST);
}