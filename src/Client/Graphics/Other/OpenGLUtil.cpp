#include "OpenGLUtil.hpp"

#include "../../../define.h"

#ifdef SDL_PLATFORM_VITA
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include "../../../GLES2_typedefs.h"
#else
#include <SDL3/SDL_opengl.h>
#include <SDL3/SDL_opengl_glext.h>
#endif

bool Game::Graphics::OpenGL::Util::ExtensionSupported(const char* Name) {
#ifdef SDL_PLATFORM_VITA
    return std::string(
               (const char*)Game::Graphics::OpenGLFunctions::glGetString(
                   GL_EXTENSIONS))
               .find(Name) != std::string::npos;
#else
    return SDL_GL_ExtensionSupported(Name);
#endif
}

void (*Game::Graphics::OpenGL::Util::GetProcAddress(const char* FunctionName))() {
#ifdef SDL_PLATFORM_VITA
    return eglGetProcAddress(FunctionName);
#else
    return SDL_GL_GetProcAddress(FunctionName);
#endif
}