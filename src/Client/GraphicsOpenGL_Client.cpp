#include "../define.h"

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

bool Game::Graphics::OpenGL::ExtensionSupported(const char* Name) {
#ifdef SDL_PLATFORM_VITA
    return std::string(
               (const char*)Game::Graphics::OpenGLFunctions::glGetString(
                   GL_EXTENSIONS))
               .find(Name) != std::string::npos;
#else
    return SDL_GL_ExtensionSupported(Name);
#endif
}

void (*Game::Graphics::OpenGL::GetProcAddress(const char* FunctionName))() {
#ifdef SDL_PLATFORM_VITA
    return eglGetProcAddress(FunctionName);
#else
    return SDL_GL_GetProcAddress(FunctionName);
#endif
}