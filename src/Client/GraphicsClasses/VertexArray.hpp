#pragma once

#include "../../define.h"

#include <cstddef>
#include <iostream>
#include <map>

#include <SDL3/SDL_platform_defines.h>
#include <SDL3/SDL_video.h>

#ifdef SDL_PLATFORM_VITA
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include "../../GLES2_typedefs.h"
#else
#include <SDL3/SDL_opengl.h>
#include <SDL3/SDL_opengl_glext.h>
#endif

#include "../GraphicsOpenGL_Client.hpp"

#include "BufferBase/OpenGL_BufferBase.hpp"
#include "VertexArray/Base_VertexArray.hpp"
#include "VertexArray/OpenGL_VertexArray.hpp"

namespace Game::Graphics::VertexArray {
    
    extern Game::Graphics::Classes::VertexArray* (*Create)();
    extern void (*CreateBulk)(
        size_t CreateAmount,
        Game::Graphics::Classes::VertexArray* VertexArrays[]);
    extern void (*Unbind)();

    extern void (*AddFloatAttribute)(int AttributeIndex, size_t Count);
    extern void (*AddIntAttribute)(int AttributeIndex, size_t Count);

    inline void Init_OpenGL();
    inline void Init_Vulkan();
    inline void Init_Metal();
    inline void Init_DirectX11();
    inline void Init_DirectX12();
}

void Game::Graphics::VertexArray::Init_OpenGL() {
    using namespace Game::Graphics::OpenGLFunctions;

    Game::Graphics::VertexArray::Create = Wrappers::Create_OpenGL;
    Game::Graphics::VertexArray::CreateBulk =
        Wrappers::CreateBulk_OpenGL;

    Game::Graphics::VertexArray::Unbind = Wrappers::Unbind_OpenGL;

    LOAD_OPENGL_FUNCTION(glEnableVertexAttribArray);
    LOAD_OPENGL_FUNCTION(glDisableVertexAttribArray);

    LOAD_OPENGL_FUNCTION(glVertexAttribPointer);

#ifndef SDL_PLATFORM_VITA
    // modern open gl
    GLint num_extensions = 0;
    Game::Graphics::OpenGLFunctions::glGetIntegerv(GL_NUM_EXTENSIONS,
                                                   &num_extensions);
    if (num_extensions > 0) {
        for (GLint i = 0; i < num_extensions; ++i) {
            const char* ext = reinterpret_cast<const char*>(
                Game::Graphics::OpenGLFunctions::glGetStringi(GL_EXTENSIONS,
                                                              i));
            if (ext) {
                std::cout << ext << "\n";
            }
        }
    } else
#endif
    // old openGL / gles 2.0
    {
        const char* gl_exts = reinterpret_cast<const char*>(
            Game::Graphics::OpenGLFunctions::glGetString(GL_EXTENSIONS));
        if (gl_exts) {
            std::cout << gl_exts << "\n";
        } else {
            std::cout << "Failed to retrieve OpenGL extensions.\n";
        }
    }

#ifdef SDL_PLATFORM_VITA
    /*
        OpenGLFunctions::glGenVertexArrays =
            (decltype(OpenGLFunctions::glGenVertexArrays))
                Game::Graphics::OpenGL::GetProcAddress("glGenVertexArraysOES");

        std::cout << "loaded glGenVertexArrays: "
                  << OpenGLFunctions::glGenVertexArrays << std::endl;

        OpenGLFunctions::glDeleteVertexArrays =
            (decltype(OpenGLFunctions::glDeleteVertexArrays))
                Game::Graphics::OpenGL::GetProcAddress("glDeleteVertexArraysOES");
        OpenGLFunctions::glBindVertexArray =
            (decltype(OpenGLFunctions::glBindVertexArray))
                Game::Graphics::OpenGL::GetProcAddress("glBindVertexArrayOES");
                */

    OpenGLFunctions::glGenVertexArrays =
        (decltype(OpenGLFunctions::glGenVertexArrays))glGenVertexArraysOES;

    std::cout << "loaded glGenVertexArrays: "
              << OpenGLFunctions::glGenVertexArrays << std::endl;

    OpenGLFunctions::glDeleteVertexArrays =
        (decltype(OpenGLFunctions::glDeleteVertexArrays))
            glDeleteVertexArraysOES;
    OpenGLFunctions::glBindVertexArray =
        (decltype(OpenGLFunctions::glBindVertexArray))glBindVertexArrayOES;

    if (OpenGLFunctions::glGenVertexArrays == NULL ||
        OpenGLFunctions::glDeleteVertexArrays == NULL ||
        OpenGLFunctions::glBindVertexArray == NULL) {
        std::clog << "Failed to load one of the functions "
                     "from 'GL_OES_vertex_array_object', using emulated VAOs."
                  << std::endl;

        ::_old_BindBuffer = Game::Graphics::OpenGLFunctions::glBindBuffer;
        ::_old_VertexAttribPointer =
            Game::Graphics::OpenGLFunctions::glVertexAttribPointer;

        ::_old_EnableVertexAttribArray =
            Game::Graphics::OpenGLFunctions::glEnableVertexAttribArray;
        ::_old_DisableVertexAttribArray =
            Game::Graphics::OpenGLFunctions::glDisableVertexAttribArray;

        Game::Graphics::OpenGLFunctions::glGenVertexArrays =
            _glGenVertexArrays_GLes20;
        Game::Graphics::OpenGLFunctions::glDeleteVertexArrays =
            _glDeleteVertexArrays_GLes20;

        Game::Graphics::OpenGLFunctions::glBindVertexArray =
            _glBindVertexArray_GLes20;
        Game::Graphics::OpenGLFunctions::glBindBuffer = _glBindBuffer_GLes20;

        Game::Graphics::OpenGLFunctions::glVertexAttribPointer =
            _glVertexAttribPointer_GLes20;
        Game::Graphics::OpenGLFunctions::glEnableVertexAttribArray =
            _glEnableVertexAttribArray_GLes20;
        Game::Graphics::OpenGLFunctions::glDisableVertexAttribArray =
            _glDisableVertexAttribArray_GLes20;
    }
#else

    int MajorVersion;
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &MajorVersion);
    if (MajorVersion >= 3) {
        LOAD_OPENGL_FUNCTION(glGenVertexArrays);
        LOAD_OPENGL_FUNCTION(glDeleteVertexArrays);

        LOAD_OPENGL_FUNCTION(glBindVertexArray);

    } else {
        int ProfileIsES;
        SDL_GL_GetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, &ProfileIsES);
        ProfileIsES = ProfileIsES == SDL_GL_CONTEXT_PROFILE_ES;

        const char* VAOExtensionName = ProfileIsES
                                           ? "GL_OES_vertex_array_object"
                                           : "GL_ARB_vertex_array_object";

        if (Game::Graphics::OpenGL::ExtensionSupported(VAOExtensionName)) {
            std::cout << "OpenGL version isn't 3.0+, but your GPU "
                         "supports the extension "
                      << VAOExtensionName << ". (you're all good!)"
                      << std::endl;

            Game::Graphics::OpenGLFunctions::glGenVertexArrays =
                (decltype(Game::Graphics::OpenGLFunctions::glGenVertexArrays))
                    Game::Graphics::OpenGL::GetProcAddress(
                        ProfileIsES ? "glGenVertexArraysOES"
                                    : "glGenVertexArraysARB");

            Game::Graphics::OpenGLFunctions::glDeleteVertexArrays =
                (decltype(Game::Graphics::OpenGLFunctions::
                              glDeleteVertexArrays))
                    Game::Graphics::OpenGL::GetProcAddress(
                        ProfileIsES ? "glDeleteVertexArraysOES"
                                    : "glDeleteVertexArraysARB");
            Game::Graphics::OpenGLFunctions::glBindVertexArray =
                (decltype(Game::Graphics::OpenGLFunctions::glBindVertexArray))
                    Game::Graphics::OpenGL::GetProcAddress(
                        ProfileIsES ? "glBindVertexArrayOES"
                                    : "glBindVertexArrayARB");
        } else {
            std::cerr << "Your GPU doesn't support OpenGL 3.0+ and "
                         "neither the extension "
                      << VAOExtensionName
                      << ". Using emulated VAOs. (tldr your gpu sucks "
                         "or something is bugged, idk bro)"
                      << std::endl;

            Wrappers::_old_BindBuffer = Game::Graphics::OpenGLFunctions::glBindBuffer;
            Wrappers::_old_VertexAttribPointer =
                Game::Graphics::OpenGLFunctions::glVertexAttribPointer;

            Wrappers::_old_EnableVertexAttribArray =
                Game::Graphics::OpenGLFunctions::glEnableVertexAttribArray;
            Wrappers::_old_DisableVertexAttribArray =
                Game::Graphics::OpenGLFunctions::glDisableVertexAttribArray;

            Game::Graphics::OpenGLFunctions::glGenVertexArrays =
                Wrappers::_glGenVertexArrays_GLes20;
            Game::Graphics::OpenGLFunctions::glDeleteVertexArrays =
                Wrappers::_glDeleteVertexArrays_GLes20;

            Game::Graphics::OpenGLFunctions::glBindVertexArray =
                Wrappers::_glBindVertexArray_GLes20;
            Game::Graphics::OpenGLFunctions::glBindBuffer =
                Wrappers::_glBindBuffer_GLes20;

            Game::Graphics::OpenGLFunctions::glVertexAttribPointer =
                Wrappers::_glVertexAttribPointer_GLes20;
            Game::Graphics::OpenGLFunctions::glEnableVertexAttribArray =
                Wrappers::_glEnableVertexAttribArray_GLes20;
            Game::Graphics::OpenGLFunctions::glDisableVertexAttribArray =
                Wrappers::_glDisableVertexAttribArray_GLes20;
        }
    }
#endif
}

void Game::Graphics::VertexArray::Init_Vulkan() {}

void Game::Graphics::VertexArray::Init_Metal() {}

void Game::Graphics::VertexArray::Init_DirectX11() {}

void Game::Graphics::VertexArray::Init_DirectX12() {}