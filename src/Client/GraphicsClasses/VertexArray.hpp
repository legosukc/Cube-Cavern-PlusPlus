#ifndef GRAPHICS_VERTEXARRAY_H
#define GRAPHICS_VERTEXARRAY_H

#include "../../define.h"

#include <cstddef>
#include <iostream>
#include <map>

#include <SDL3/SDL_opengl.h>
#include <SDL3/SDL_opengl_glext.h>
#include <SDL3/SDL_video.h>

#include "BaseClassDefinitions.hpp"
#include "BufferBase.hpp"

namespace Game::Graphics::OpenGLFunctions {
    PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
    PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays;

    PFNGLBINDVERTEXARRAYPROC glBindVertexArray;

    PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
    PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray;

    PFNGLVERTEXATTRIBPOINTERARBPROC glVertexAttribPointer;
}

namespace Game::Graphics::Classes {

    class VertexArray : public Game::Graphics::Classes::OpenGLClassBase {
       public:
        virtual ~VertexArray();

        virtual void Bind();
        virtual bool IsBound() const;
    };

    class VertexArray_OpenGL : public VertexArray {
       public:
        virtual ~VertexArray_OpenGL() override;

        virtual void Bind() override;
        virtual bool IsBound() const override;
    };
}

namespace Game::Graphics::VertexArray {

    extern Game::Graphics::Classes::VertexArray* (*Create)();
    extern void (*CreateBulk)(
        size_t CreateAmount,
        Game::Graphics::Classes::VertexArray* VertexArrays[]);
    extern void (*Unbind)();

    inline void Init_OpenGL();
    inline void Init_Vulkan();
    inline void Init_Metal();
    inline void Init_DirectX11();
    inline void Init_DirectX12();
}

Game::Graphics::Classes::VertexArray::~VertexArray() = default;

void Game::Graphics::Classes::VertexArray::Bind() {}

bool Game::Graphics::Classes::VertexArray::IsBound() const {
    return false;
}

Game::Graphics::Classes::VertexArray_OpenGL::~VertexArray_OpenGL() {
    if (this->GLObject == 0) {
#ifdef DEBUG_BUILD
        std::cerr << "Attempted to free a unallocated OpenGL "
                     "VertexArray (VertexArray_OpenGL)."
                  << std::endl;
        __debugbreak();
#endif
        return;
    }
    Game::Graphics::OpenGLFunctions::glDeleteVertexArrays(1, &this->GLObject);
}

void Game::Graphics::Classes::VertexArray_OpenGL::Bind() {
    Game::Graphics::OpenGLFunctions::glBindVertexArray(this->GLObject);
}

bool Game::Graphics::Classes::VertexArray_OpenGL::IsBound() const {
    // TODO: FIX THIS!!!!
    return true;
}

namespace {

    static Game::Graphics::Classes::VertexArray* _VertexArray_Create_OpenGL() {
        Game::Graphics::Classes::VertexArray_OpenGL* NewVertexArray =
            new Game::Graphics::Classes::VertexArray_OpenGL();
        Game::Graphics::OpenGLFunctions::glGenVertexArrays(
            1, &NewVertexArray->GLObject);

        return reinterpret_cast<Game::Graphics::Classes::VertexArray*>(
            NewVertexArray);
    }

    static void _VertexArray_CreateBulk_OpenGL(
        size_t CreateAmount,
        Game::Graphics::Classes::VertexArray* VertexArrays[]) {
        for (size_t i = 0; i < CreateAmount; ++i) {
            VertexArrays[i] =
                reinterpret_cast<Game::Graphics::Classes::VertexArray*>(
                    new Game::Graphics::Classes::VertexArray_OpenGL());
            Game::Graphics::OpenGLFunctions::glGenVertexArrays(
                1, &VertexArrays[i]->GLObject);
        }
    }

    static void _VertexArray_Unbind_OpenGL() {
        Game::Graphics::OpenGLFunctions::glBindVertexArray(0);
    }

    struct _VAO_AttributePointerStruct {
        bool Enabled = false;

        GLint size = 0;
        GLenum type = 0;
        GLboolean normalized = false;
        GLsizei stride = 0;
        const void* pointer = NULL;
    };

    struct _VAO_Struct {
        GLuint VBO, EBO;

        _VAO_AttributePointerStruct Attributes[32];
    };
    std::map<GLuint, _VAO_Struct> _VAO_Attribs;

    GLuint BoundVAO = 0;

    static PFNGLVERTEXATTRIBPOINTERARBPROC _old_VertexAttribPointer;
    static PFNGLBINDBUFFERPROC _old_BindBuffer;

    static PFNGLENABLEVERTEXATTRIBARRAYPROC _old_EnableVertexAttribArray;
    static PFNGLDISABLEVERTEXATTRIBARRAYPROC _old_DisableVertexAttribArray;

    static void _glGenVertexArrays_GLes20(GLsizei count, GLuint* arrays) {
        Game::Graphics::OpenGLFunctions::glGenBuffers(count, arrays);

        for (; count > 0; --count) {
            ::_VAO_Attribs[arrays[count]];
        }
    }

    static void _glBindBuffer_GLes20(GLenum target, GLuint buffer) {
        _old_BindBuffer(target, buffer);

        if (::BoundVAO == 0) {
            return;
        }

        _VAO_Struct& VAO = ::_VAO_Attribs[::BoundVAO];
        switch (target) {
            case GL_ARRAY_BUFFER:
                VAO.VBO = buffer;
                break;
            case GL_ELEMENT_ARRAY_BUFFER:
                VAO.EBO = buffer;
                break;
        }
    }

    static void _glBindVertexArray_GLes20(GLuint array) {
        if (array == 0) {
            ::BoundVAO = 0;
            return;
        }

        if (array == ::BoundVAO || ::_VAO_Attribs.count(array) == 0) {
            return;
        }

        const _VAO_Struct& VAO = ::_VAO_Attribs[array];
        _old_BindBuffer(GL_ARRAY_BUFFER, VAO.VBO);
        _old_BindBuffer(GL_ELEMENT_ARRAY_BUFFER, VAO.EBO);

        const _VAO_AttributePointerStruct* AttribPointer;
        for (GLuint index = 0; index < (sizeof(VAO.Attributes) /
                                        sizeof(::_VAO_AttributePointerStruct));
             ++index) {
            AttribPointer = &VAO.Attributes[index];
            if (!AttribPointer->Enabled) {
                ::_old_DisableVertexAttribArray(index);
                continue;
            }
            ::_old_EnableVertexAttribArray(index);

            ::_old_VertexAttribPointer(
                index, AttribPointer->size, AttribPointer->type,
                AttribPointer->normalized, AttribPointer->stride,
                AttribPointer->pointer);
        }

        ::BoundVAO = array;
    }

    static void _glVertexAttribPointer_GLes20(GLuint index,
                                              GLint size,
                                              GLenum type,
                                              GLboolean normalized,
                                              GLsizei stride,
                                              const void* pointer) {
        if (::BoundVAO == 0) {
            //::_old_VertexAttribPointer(index, size, type, normalized, stride,
            //: pointer);
            return;
        }

        _VAO_AttributePointerStruct& AttribPointer =
            ::_VAO_Attribs[::BoundVAO].Attributes[index];
        AttribPointer.size = size;
        AttribPointer.type = type;
        AttribPointer.normalized = normalized;
        AttribPointer.stride = stride;
        AttribPointer.pointer = pointer;
    }

    static void _glEnableVertexAttribArray_GLes20(GLuint index) {
        if (::BoundVAO == 0) {
            //::_old_EnableVertexAttribArray(index);
            return;
        }
        ::_VAO_Attribs[::BoundVAO].Attributes[index].Enabled = true;
    }

    static void _glDisableVertexAttribArray_GLes20(GLuint index) {
        if (::BoundVAO == 0) {
            //::_old_DisableVertexAttribArray(index);
            return;
        }
        ::_VAO_Attribs[::BoundVAO].Attributes[index].Enabled = false;
    }

    static void _glDeleteVertexArrays_GLes20(GLsizei count,
                                             const GLuint* arrays) {
        for (GLsizei i = 0; i < count; ++i) {
            ::_VAO_Attribs.erase(arrays[i]);
        }
        ::Game::Graphics::OpenGLFunctions::glDeleteBuffers(count, arrays);
    }
}

namespace Game::Graphics::VertexArray {

    Game::Graphics::Classes::VertexArray* (*Create)();
    void (*CreateBulk)(size_t CreateAmount,
                       Game::Graphics::Classes::VertexArray* VertexArrays[]);
    void (*Unbind)();
}

void Game::Graphics::VertexArray::Init_OpenGL() {
    Game::Graphics::VertexArray::Create = ::_VertexArray_Create_OpenGL;
    Game::Graphics::VertexArray::CreateBulk = ::_VertexArray_CreateBulk_OpenGL;

    Game::Graphics::VertexArray::Unbind = ::_VertexArray_Unbind_OpenGL;

    LOAD_OPENGL_FUNCTION(glEnableVertexAttribArray);
    LOAD_OPENGL_FUNCTION(glDisableVertexAttribArray);

    LOAD_OPENGL_FUNCTION(glVertexAttribPointer);

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
        if (SDL_GL_ExtensionSupported(VAOExtensionName)) {
            std::cout << "OpenGL version isn't 3.0+, but your GPU "
                         "supports the extension "
                      << VAOExtensionName << ". (you're all good!)" << std::endl;

            Game::Graphics::OpenGLFunctions::glGenVertexArrays =
                (PFNGLGENVERTEXARRAYSPROC)SDL_GL_GetProcAddress(
                    ProfileIsES ? "glGenVertexArraysOES"
                                : "glGenVertexArraysARB");
            Game::Graphics::OpenGLFunctions::glDeleteVertexArrays =
                (PFNGLDELETEVERTEXARRAYSPROC)SDL_GL_GetProcAddress(
                    ProfileIsES ? "glDeleteVertexArraysOES"
                                : "glDeleteVertexArraysARB");
            Game::Graphics::OpenGLFunctions::glBindVertexArray =
                (PFNGLBINDVERTEXARRAYPROC)SDL_GL_GetProcAddress(
                    ProfileIsES ? "glBindVertexArrayOES"
                                : "glBindVertexArrayARB");

        } else {
            std::cerr << "Your GPU doesn't support OpenGL 3.0+ and "
                         "neither the extension "
                      << VAOExtensionName
                      << ". Using emulated VAOs. (tldr your gpu sucks "
                         "or something is bugged, idk bro)"
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
    }
}

void Game::Graphics::VertexArray::Init_Vulkan() {}

void Game::Graphics::VertexArray::Init_Metal() {}

void Game::Graphics::VertexArray::Init_DirectX11() {}

void Game::Graphics::VertexArray::Init_DirectX12() {}

#endif