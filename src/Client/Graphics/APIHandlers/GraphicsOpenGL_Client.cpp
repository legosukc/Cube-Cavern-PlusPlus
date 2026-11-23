#include "../../../define.h"

#include <SDL3/SDL_platform_defines.h>

#ifdef SDL_PLATFORM_VITA
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <string>
#else
#include <SDL3/SDL_video.h>
#endif

#include "GraphicsOpenGL_Client.hpp"

namespace Game::Graphics::OpenGLFunctions {

    PFNGLVIEWPORTPROC glViewport = NULL;

    PFNGLENABLEPROC glEnable = NULL;
    PFNGLDISABLEPROC glDisable = NULL;

    PFNGLCLEARCOLORPROC glClearColor = NULL;
    PFNGLCLEARPROC glClear = NULL;

    PFNGLBLENDFUNCPROC glBlendFunc = NULL;

    PFNGLGETINTEGERVPROC glGetIntegerv = NULL;
    PFNGLGETSTRINGPROC glGetString = NULL;

#ifndef SDL_PLATFORM_VITA
    PFNGLGETSTRINGIPROC glGetStringi = NULL;
#endif

    PFNGLISENABLEDPROC glIsEnabled = NULL;

    PFNGLDRAWARRAYSPROC glDrawArrays = NULL;
    PFNGLDRAWELEMENTSPROC glDrawElements = NULL;

#ifndef SDL_PLATFORM_VITA
    PFNGLDRAWARRAYSINSTANCEDPROC glDrawArraysInstanced = NULL;
    PFNGLDRAWELEMENTSINSTANCEDPROC glDrawElementsInstanced = NULL;
#endif
}

namespace Game::Graphics::OpenGL {
    int MajorVersion = -1;
    int MinorVersion = -1;
    int Profile = -1;
}

void Game::Graphics::OpenGL::Wrappers::ClearColorBuffer(float R,
                                                        float G,
                                                        float B,
                                                        float A) {
    Game::Graphics::OpenGLFunctions::glClearColor(R, G, B, A);
    Game::Graphics::OpenGLFunctions::glClear(GL_COLOR_BUFFER_BIT);
}

void Game::Graphics::OpenGL::Wrappers::ClearDepthBuffer() {
    Game::Graphics::OpenGLFunctions::glClear(GL_DEPTH_BUFFER_BIT);
}

void Game::Graphics::OpenGL::Wrappers::ClearStencilBuffer() {
    Game::Graphics::OpenGLFunctions::glClear(GL_STENCIL_BUFFER_BIT);
}

void Game::Graphics::OpenGL::Wrappers::ClearAllBuffers(float R,
                                                       float G,
                                                       float B,
                                                       float A) {
    Game::Graphics::OpenGLFunctions::glClearColor(R, G, B, A);
    Game::Graphics::OpenGLFunctions::glClear(
        GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void Game::Graphics::OpenGL::Wrappers::WindowResizedEvent_UpdateGLViewport(
    const Math::IVector2& Size) {
    Game::Graphics::OpenGLFunctions::glViewport(0, 0, Size.X, Size.Y);
}