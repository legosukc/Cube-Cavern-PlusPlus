#pragma once

#include "../../../../define.h"

#ifdef SDL_PLATFORM_VITA
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include "../../../GLES2_typedefs.h"
#else
#include <SDL3/SDL_opengl.h>
#include <SDL3/SDL_opengl_glext.h>
#endif

#include "../BaseClassDefinitions.hpp"
#include "Base_VertexArray.hpp"

#include <map>

#include "../BufferBase/OpenGL_BufferBase.hpp"
#include "../VertexArray.hpp"

#include "../../Other/OpenGLUtil.hpp"

namespace Game::Graphics::OpenGLFunctions {
    extern void (*glGenVertexArrays)(GLsizei n, GLuint* arrays);
    extern void (*glDeleteVertexArrays)(GLsizei n, const GLuint* arrays);

    extern void (*glBindVertexArray)(GLuint array);

    extern PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
    extern PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray;

    extern PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;
}

namespace Game::Graphics::VertexArray::OpenGL::Wrappers {
    Game::Graphics::Classes::VertexArray* Create();

    void CreateBulk(size_t CreateAmount,
                    Game::Graphics::Classes::VertexArray* VertexArrays[]);

    void Unbind();

    void AddFloatAttribute(int AttributeIndex, size_t Count, size_t Stride);
    void AddIntAttribute(int AttributeIndex, size_t Count, size_t Stride);

    struct VAOAttributePointer {
        bool Enabled = false;

        GLint size = 0;
        GLenum type = 0;
        GLboolean normalized = false;
        GLsizei stride = 0;
        const void* pointer = NULL;
    };

    struct VAOData {
        GLuint VBO, EBO;
        VAOAttributePointer Attributes[32];
    };

    extern std::map<GLuint, VAOData> VAO_Attribs;

    extern GLuint BoundVAO;

    extern PFNGLVERTEXATTRIBPOINTERPROC _old_VertexAttribPointer;
    extern PFNGLBINDBUFFERPROC _old_BindBuffer;

    extern PFNGLENABLEVERTEXATTRIBARRAYPROC _old_EnableVertexAttribArray;
    extern PFNGLDISABLEVERTEXATTRIBARRAYPROC _old_DisableVertexAttribArray;

    void _glGenVertexArrays_GLes20(GLsizei count, GLuint* arrays);

    void _glBindBuffer_GLes20(GLenum target, GLuint buffer);

    void _glBindVertexArray_GLes20(GLuint array);

    void _glVertexAttribPointer_GLes20(GLuint index,
                                       GLint size,
                                       GLenum type,
                                       GLboolean normalized,
                                       GLsizei stride,
                                       const void* pointer);

    void _glEnableVertexAttribArray_GLes20(GLuint index);

    void _glDisableVertexAttribArray_GLes20(GLuint index);

    void _glDeleteVertexArrays_GLes20(GLsizei count, const GLuint* arrays);

    void _glBindVertexArrayOES_wrapper(GLuint vao);
}

namespace Game::Graphics::Classes {

    class VertexArray_OpenGL : public VertexArray,
                               public Game::Graphics::Classes::OpenGLClassBase {
       public:
        virtual ~VertexArray_OpenGL() override;

        virtual void Bind() override;
        virtual bool IsBound() const override;
    };
}

namespace Game::Graphics::VertexArray::OpenGL {
    inline void Init() {
        using namespace Game::Graphics::OpenGLFunctions;

        Game::Graphics::VertexArray::Create = Wrappers::Create;
        Game::Graphics::VertexArray::CreateBulk = Wrappers::CreateBulk;

        Game::Graphics::VertexArray::Unbind = Wrappers::Unbind;

        Game::Graphics::VertexArray::AddFloatAttribute =
            Wrappers::AddFloatAttribute;
        Game::Graphics::VertexArray::AddIntAttribute =
            Wrappers::AddIntAttribute;

        LOAD_OPENGL_FUNCTION(glEnableVertexAttribArray);
        LOAD_OPENGL_FUNCTION(glDisableVertexAttribArray);

        LOAD_OPENGL_FUNCTION(glVertexAttribPointer);

        /*
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
        }*/

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
            std::clog
                << "Failed to load one of the functions "
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
            Game::Graphics::OpenGLFunctions::glBindBuffer =
                _glBindBuffer_GLes20;

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

            if (Game::Graphics::OpenGL::Util::ExtensionSupported(
                    VAOExtensionName)) {
                std::cout << "OpenGL version isn't 3.0+, but your GPU "
                             "supports the extension "
                          << VAOExtensionName << ". (you're all good!)"
                          << std::endl;

                Game::Graphics::OpenGLFunctions::glGenVertexArrays =
                    (decltype(Game::Graphics::OpenGLFunctions::
                                  glGenVertexArrays))
                        Game::Graphics::OpenGL::Util::GetProcAddress(
                            ProfileIsES ? "glGenVertexArraysOES"
                                        : "glGenVertexArraysARB");

                Game::Graphics::OpenGLFunctions::glDeleteVertexArrays =
                    (decltype(Game::Graphics::OpenGLFunctions::
                                  glDeleteVertexArrays))
                        Game::Graphics::OpenGL::Util::GetProcAddress(
                            ProfileIsES ? "glDeleteVertexArraysOES"
                                        : "glDeleteVertexArraysARB");
                Game::Graphics::OpenGLFunctions::glBindVertexArray =
                    (decltype(Game::Graphics::OpenGLFunctions::
                                  glBindVertexArray))
                        Game::Graphics::OpenGL::Util::GetProcAddress(
                            ProfileIsES ? "glBindVertexArrayOES"
                                        : "glBindVertexArrayARB");
            } else {
                std::cerr << "Your GPU doesn't support OpenGL 3.0+ and "
                             "neither the extension "
                          << VAOExtensionName
                          << ". Using emulated VAOs. (tldr your gpu sucks "
                             "or something is bugged, idk bro)"
                          << std::endl;

                Wrappers::_old_BindBuffer =
                    Game::Graphics::OpenGLFunctions::glBindBuffer;
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
}