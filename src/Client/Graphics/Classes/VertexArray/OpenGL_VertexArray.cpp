#include "OpenGL_VertexArray.hpp"

#include "../BufferBase/OpenGL_BufferBase.hpp"

namespace Game::Graphics::OpenGLFunctions {
    void (*glGenVertexArrays)(GLsizei n, GLuint* arrays) = NULL;
    void (*glDeleteVertexArrays)(GLsizei n, const GLuint* arrays) = NULL;

    void (*glBindVertexArray)(GLuint array) = NULL;

    PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray = NULL;
    PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray = NULL;

    PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer = NULL;
}

Game::Graphics::Classes::VertexArray*
    Game::Graphics::VertexArray::OpenGL::Wrappers::Create() {
    Game::Graphics::Classes::VertexArray_OpenGL* NewVertexArray =
        new Game::Graphics::Classes::VertexArray_OpenGL();
    Game::Graphics::OpenGLFunctions::glGenVertexArrays(
        1, &NewVertexArray->GLObject);

    return reinterpret_cast<Game::Graphics::Classes::VertexArray*>(
        NewVertexArray);
}

void Game::Graphics::VertexArray::OpenGL::Wrappers::CreateBulk(
    size_t CreateAmount,
    Game::Graphics::Classes::VertexArray* VertexArrays[]) {
    for (size_t i = 0; i < CreateAmount; ++i) {
        VertexArrays[i] =
            reinterpret_cast<Game::Graphics::Classes::VertexArray*>(
                new Game::Graphics::Classes::VertexArray_OpenGL());
        Game::Graphics::OpenGLFunctions::glGenVertexArrays(
            1, &reinterpret_cast<Game::Graphics::Classes::VertexArray_OpenGL*>(
                    VertexArrays)[i]
                    .GLObject);
    }
}

void Game::Graphics::VertexArray::OpenGL::Wrappers::Unbind() {
    Game::Graphics::OpenGLFunctions::glBindVertexArray(0);
}

void Game::Graphics::VertexArray::OpenGL::Wrappers::AddFloatAttribute(
    int AttributeIndex,
    size_t Count,
    size_t Stride) {
    Game::Graphics::OpenGLFunctions::glEnableVertexAttribArray(AttributeIndex);
    Game::Graphics::OpenGLFunctions::glVertexAttribPointer(
        AttributeIndex, Count, GL_FLOAT, GL_FALSE, static_cast<GLsizei>(Stride),
        (const void*)0);
}

void Game::Graphics::VertexArray::OpenGL::Wrappers::AddIntAttribute(
    int AttributeIndex,
    size_t Count,
    size_t Stride) {
    Game::Graphics::OpenGLFunctions::glEnableVertexAttribArray(AttributeIndex);
    Game::Graphics::OpenGLFunctions::glVertexAttribPointer(
        AttributeIndex, Count, GL_INT, GL_FALSE, static_cast<GLsizei>(Stride),
        (const void*)0);
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

namespace Game::Graphics::VertexArray::OpenGL::Wrappers {

    std::map<GLuint, VAOData> VAO_Attribs;

    GLuint BoundVAO = 0;

    PFNGLVERTEXATTRIBPOINTERPROC _old_VertexAttribPointer = NULL;
    PFNGLBINDBUFFERPROC _old_BindBuffer = NULL;

    PFNGLENABLEVERTEXATTRIBARRAYPROC _old_EnableVertexAttribArray = NULL;
    PFNGLDISABLEVERTEXATTRIBARRAYPROC _old_DisableVertexAttribArray = NULL;
}

void Game::Graphics::VertexArray::OpenGL::Wrappers::_glGenVertexArrays_GLes20(
    GLsizei count,
    GLuint* arrays) {
    Game::Graphics::OpenGLFunctions::glGenBuffers(count, arrays);

    for (; count > 0; --count) {
        Wrappers::VAO_Attribs[arrays[count]];
    }
}

void Game::Graphics::VertexArray::OpenGL::Wrappers::_glBindBuffer_GLes20(
    GLenum target,
    GLuint buffer) {
    Wrappers::_old_BindBuffer(target, buffer);

    if (Wrappers::BoundVAO == 0) {
        return;
    }

    VAOData& VAO = Wrappers::VAO_Attribs[Wrappers::BoundVAO];
    switch (target) {
        case GL_ARRAY_BUFFER:
            VAO.VBO = buffer;
            break;
        case GL_ELEMENT_ARRAY_BUFFER:
            VAO.EBO = buffer;
            break;
    }
}

void Game::Graphics::VertexArray::OpenGL::Wrappers::_glBindVertexArray_GLes20(
    GLuint array) {
    if (array == 0) {
        Wrappers::BoundVAO = 0;
        return;
    }

    if (array == Wrappers::BoundVAO ||
        Wrappers::VAO_Attribs.count(array) == 0) {
        return;
    }

    const VAOData& VAO = Wrappers::VAO_Attribs[array];
    _old_BindBuffer(GL_ARRAY_BUFFER, VAO.VBO);
    _old_BindBuffer(GL_ELEMENT_ARRAY_BUFFER, VAO.EBO);

    const VAOAttributePointer* AttribPointer;
    for (GLuint index = 0; index < (sizeof(VAO.Attributes) /
                                    sizeof(Wrappers::VAOAttributePointer));
         ++index) {
        AttribPointer = &VAO.Attributes[index];
        if (!AttribPointer->Enabled) {
            Wrappers::_old_DisableVertexAttribArray(index);
            continue;
        }
        Wrappers::_old_EnableVertexAttribArray(index);

        Wrappers::_old_VertexAttribPointer(
            index, AttribPointer->size, AttribPointer->type,
            AttribPointer->normalized, AttribPointer->stride,
            AttribPointer->pointer);
    }

    Wrappers::BoundVAO = array;
}

void Game::Graphics::VertexArray::OpenGL::Wrappers::
    _glVertexAttribPointer_GLes20(GLuint index,
                                  GLint size,
                                  GLenum type,
                                  GLboolean normalized,
                                  GLsizei stride,
                                  const void* pointer) {
    if (Wrappers::BoundVAO == 0) {
        // Wrappers::_old_VertexAttribPointer(index, size, type, normalized,
        //: stride,
        //: pointer);
        return;
    }

    VAOAttributePointer& AttribPointer =
        Wrappers::VAO_Attribs[Wrappers::BoundVAO].Attributes[index];
    AttribPointer.size = size;
    AttribPointer.type = type;
    AttribPointer.normalized = normalized;
    AttribPointer.stride = stride;
    AttribPointer.pointer = pointer;
}

void Game::Graphics::VertexArray::OpenGL::Wrappers::
    _glEnableVertexAttribArray_GLes20(GLuint index) {
    if (Wrappers::BoundVAO == 0) {
        // Wrappers::_old_EnableVertexAttribArray(index);
        return;
    }
    Wrappers::VAO_Attribs[Wrappers::BoundVAO].Attributes[index].Enabled = true;
}

void Game::Graphics::VertexArray::OpenGL::Wrappers::
    _glDisableVertexAttribArray_GLes20(GLuint index) {
    if (Wrappers::BoundVAO == 0) {
        //::_old_DisableVertexAttribArray(index);
        return;
    }
    Wrappers::VAO_Attribs[Wrappers::BoundVAO].Attributes[index].Enabled = false;
}

void Game::Graphics::VertexArray::OpenGL::Wrappers::
    _glDeleteVertexArrays_GLes20(GLsizei count, const GLuint* arrays) {
    for (GLsizei i = 0; i < count; ++i) {
        Wrappers::VAO_Attribs.erase(arrays[i]);
    }
    Game::Graphics::OpenGLFunctions::glDeleteBuffers(count, arrays);
}

void Game::Graphics::VertexArray::OpenGL::Wrappers::
    _glBindVertexArrayOES_wrapper(GLuint vao) {}