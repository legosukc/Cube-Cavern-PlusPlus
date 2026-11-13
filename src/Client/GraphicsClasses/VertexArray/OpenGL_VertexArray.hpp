#pragma once

#include "../BaseClassDefinitions.hpp"
#include "Base_VertexArray.hpp"

#include <map>

namespace Game::Graphics::OpenGLFunctions {
    extern void (*glGenVertexArrays)(GLsizei n, GLuint* arrays);
    extern void (*glDeleteVertexArrays)(GLsizei n, const GLuint* arrays);

    extern void (*glBindVertexArray)(GLuint array);

    extern PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
    extern PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray;

    extern PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;
}

namespace Game::Graphics::VertexArray::Wrappers {
    Game::Graphics::Classes::VertexArray* Create_OpenGL();

    void CreateBulk_OpenGL(
        size_t CreateAmount,
        Game::Graphics::Classes::VertexArray* VertexArrays[]);

    void Unbind_OpenGL();

    void AddFloatAttribute_OpenGL(int AttributeIndex, size_t Count);
    void AddIntAttribute_OpenGL(int AttributeIndex, size_t Count);

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

    extern std::map<GLuint, _VAO_Struct> _VAO_Attribs;

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