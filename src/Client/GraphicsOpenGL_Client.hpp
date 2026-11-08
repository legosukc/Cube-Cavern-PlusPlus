#pragma once

#include "../define.h"

#include <SDL3/SDL.h>

#ifdef SDL_PLATFORM_VITA
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include "../GLES2_typedefs.h"
#else
#include <SDL3/SDL_opengl.h>
#include <SDL3/SDL_opengl_glext.h>
#include <SDL3/SDL_opengles2_gl2ext.h>
#endif

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

namespace Game::Graphics::OpenGL {

    extern int MajorVersion;
    extern int MinorVersion;
    extern int Profile;

    bool ExtensionSupported(const char* Name);
    void (*GetProcAddress(const char* FunctionName))();
}