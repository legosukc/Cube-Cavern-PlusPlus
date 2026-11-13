#include "OpenGL_Program.hpp"

namespace Game::Graphics::OpenGLFunctions {

    PFNGLCREATEPROGRAMPROC glCreateProgram = NULL;
    PFNGLDELETEPROGRAMPROC glDeleteProgram = NULL;

    PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation = NULL;

#ifndef SDL_PLATFORM_VITA
    PFNGLGETUNIFORMBLOCKINDEXPROC glGetUniformBlockIndex = NULL;
    PFNGLUNIFORMBLOCKBINDINGPROC glUniformBlockBinding = NULL;
#endif

    PFNGLATTACHSHADERPROC glAttachShader = NULL;
    PFNGLGETATTACHEDSHADERSPROC glGetAttachedShaders = NULL;
    PFNGLDETACHSHADERPROC glDetachShader = NULL;

    PFNGLLINKPROGRAMPROC glLinkProgram = NULL;
    PFNGLUSEPROGRAMPROC glUseProgram = NULL;

    PFNGLGETPROGRAMIVPROC glGetProgramiv = NULL;

    PFNGLUNIFORM1IPROC glUniform1i = NULL;
    //  PFNGLUNIFORM2IVPROC glUniform1iv = NULL;
    PFNGLUNIFORM2IVPROC glUniform2iv = NULL;
    PFNGLUNIFORM2IVPROC glUniform3iv = NULL;
    PFNGLUNIFORM2IVPROC glUniform4iv = NULL;

#ifndef SDL_PLATFORM_VITA
    PFNGLUNIFORM1UIPROC glUniform1ui = NULL;
    //  PFNGLUNIFORM2UIVPROC glUniform1uiv = NULL;
    PFNGLUNIFORM2UIVPROC glUniform2uiv = NULL;
    PFNGLUNIFORM2UIVPROC glUniform3uiv = NULL;
    PFNGLUNIFORM2UIVPROC glUniform4uiv = NULL;
#endif

    PFNGLUNIFORM1FPROC glUniform1f = NULL;
    //  PFNGLUNIFORM2FVPROC glUniform1fv = NULL;
    PFNGLUNIFORM2FVPROC glUniform2fv = NULL;
    PFNGLUNIFORM2FVPROC glUniform3fv = NULL;
    PFNGLUNIFORM2FVPROC glUniform4fv = NULL;

    PFNGLUNIFORMMATRIX2FVPROC glUniformMatrix2fv = NULL;
    PFNGLUNIFORMMATRIX3FVPROC glUniformMatrix3fv = NULL;
    PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv = NULL;

    void (*glGetProgramBinary)(GLuint program,
                               GLsizei bufSize,
                               GLsizei* length,
                               GLenum* binaryFormat,
                               void* binary) = NULL;
}

void Game::Graphics::Program::Wrappers::OpenGL::SetUniformSint32(
    Sint32 UniformIndex,
    Sint32 Value) {
    Game::Graphics::OpenGLFunctions::glUniform1i(UniformIndex, Value);
}

void Game::Graphics::Program::Wrappers::OpenGL::SetUniformUint32(
    Sint32 UniformIndex,
    Uint32 Value) {
#ifdef SDL_PLATFORM_VITA
    Exceptions::ThrowException<Exceptions::RuntimeError>(
        "Function "
        "'Game::Graphics::Program::Wrappers::OpenGL::SetUniformUint32' is not "
        "implemented on this platform.");
#else
    Game::Graphics::OpenGLFunctions::glUniform1ui(UniformIndex, Value);
#endif
}

void Game::Graphics::Program::Wrappers::OpenGL::SetUniformBool(
    Sint32 UniformIndex,
    bool Value) {
    Game::Graphics::OpenGLFunctions::glUniform1i(UniformIndex, Value);
}

void Game::Graphics::Program::Wrappers::OpenGL::SetUniformFloat(
    Sint32 UniformIndex,
    float Value) {
    Game::Graphics::OpenGLFunctions::glUniform1f(UniformIndex, Value);
}

template <class BoolVectorType>
void Game::Graphics::Program::Wrappers::OpenGL::SetUniformBvec_(
    Sint32 UniformIndex,
    const BoolVectorType& BVector) {
    if CONSTEXPR_IF (BoolVectorType::ComponentCount == 2) {
        Game::Graphics::OpenGLFunctions::glUniform2iv(UniformIndex, 1,
                                                      (const GLint*)&BVector);
    } else if CONSTEXPR_IF (BoolVectorType::ComponentCount == 3) {
        Game::Graphics::OpenGLFunctions::glUniform3iv(UniformIndex, 1,
                                                      (const GLint*)&BVector);
    } else if CONSTEXPR_IF (BoolVectorType::ComponentCount == 4) {
        Game::Graphics::OpenGLFunctions::glUniform4iv(UniformIndex, 1,
                                                      (const GLint*)&BVector);
    }
}

template void Game::Graphics::Program::Wrappers::OpenGL::SetUniformBvec_<
    Math::IVector2>(Sint32 UniformIndex, const Math::IVector2& BVector);

template void Game::Graphics::Program::Wrappers::OpenGL::SetUniformBvec_<
    Math::IVector3>(Sint32 UniformIndex, const Math::IVector3& BVector);

template void Game::Graphics::Program::Wrappers::OpenGL::SetUniformBvec_<
    Math::IVector4>(Sint32 UniformIndex, const Math::IVector4& BVector);

template <class VectorType>
void Game::Graphics::Program::Wrappers::OpenGL::SetUniform_vec2(
    Sint32 UniformIndex,
    const VectorType& Vector) {
    using namespace Game::Graphics::OpenGLFunctions;

    if CONSTEXPR_IF (std::is_floating_point_v<
                         typename VectorType::ComponentType>) {
        Game::Graphics::OpenGLFunctions::glUniform2fv(
            UniformIndex, 1, reinterpret_cast<const float*>(&Vector));

    } else if CONSTEXPR_IF (std::is_signed_v<
                                typename VectorType::ComponentType>) {
        Game::Graphics::OpenGLFunctions::glUniform2iv(UniformIndex, 1,
                                                      &Vector.X);

    } else if CONSTEXPR_IF (std::is_unsigned_v<
                                typename VectorType::ComponentType>) {
#ifdef SDL_PLATFORM_VITA
        Exceptions::ThrowException<Exceptions::RuntimeError>(
            "Function "
            "'Game::Graphics::Program::Wrappers::OpenGL::SetUniform_vec2<Math::"
            "UVector2>' is "
            "not "
            "implemented on this platform.");
#else
        Game::Graphics::OpenGLFunctions::glUniform2uiv(UniformIndex, 1,
                                                       &Vector.X);
#endif
    }
}

template void Game::Graphics::Program::Wrappers::OpenGL::SetUniform_vec2<
    Math::IVector2>(Sint32 UniformIndex, const Math::IVector2& BVector);

template void Game::Graphics::Program::Wrappers::OpenGL::SetUniform_vec2<
    Math::UVector2>(Sint32 UniformIndex, const Math::UVector2& BVector);

template void Game::Graphics::Program::Wrappers::OpenGL::SetUniform_vec2<
    Math::Vector2>(Sint32 UniformIndex, const Math::Vector2& BVector);

template <class VectorType>
void Game::Graphics::Program::Wrappers::OpenGL::SetUniform_vec3(
    Sint32 UniformIndex,
    const VectorType& Vector) {
    using namespace Game::Graphics::OpenGLFunctions;

    if CONSTEXPR_IF (std::is_floating_point_v<
                         typename VectorType::ComponentType>) {
        Game::Graphics::OpenGLFunctions::glUniform3fv(
            UniformIndex, 1,  // VectorType::ComponentCount,
            reinterpret_cast<const float*>(&Vector));

    } else if CONSTEXPR_IF (std::is_signed_v<
                                typename VectorType::ComponentType>) {
        Game::Graphics::OpenGLFunctions::glUniform3iv(UniformIndex, 1,
                                                      &Vector.X);

    } else if CONSTEXPR_IF (std::is_unsigned_v<
                                typename VectorType::ComponentType>) {
#ifdef SDL_PLATFORM_VITA
        Exceptions::ThrowException<Exceptions::RuntimeError>(
            "Function "
            "'Game::Graphics::Program::Wrappers::OpenGL::SetUniform_vec2<Math::"
            "UVector3>' is "
            "not "
            "implemented on this platform.");
#else
        Game::Graphics::OpenGLFunctions::glUniform3uiv(UniformIndex, 1,
                                                       &Vector.X);
#endif
    }
}

template void Game::Graphics::Program::Wrappers::OpenGL::SetUniform_vec3<
    Math::IVector3>(Sint32 UniformIndex, const Math::IVector3& BVector);

template void Game::Graphics::Program::Wrappers::OpenGL::SetUniform_vec3<
    Math::UVector3>(Sint32 UniformIndex, const Math::UVector3& BVector);

template void Game::Graphics::Program::Wrappers::OpenGL::SetUniform_vec3<
    Math::Vector3>(Sint32 UniformIndex, const Math::Vector3& BVector);

template <class VectorType>
void Game::Graphics::Program::Wrappers::OpenGL::SetUniform_vec4(
    Sint32 UniformIndex,
    const VectorType& Vector) {
    using namespace Game::Graphics::OpenGLFunctions;

    if CONSTEXPR_IF (std::is_floating_point_v<
                         typename VectorType::ComponentType>) {
        Game::Graphics::OpenGLFunctions::glUniform4fv(
            UniformIndex, 1, reinterpret_cast<const float*>(&Vector));

    } else if CONSTEXPR_IF (std::is_signed_v<
                                typename VectorType::ComponentType>) {
        Game::Graphics::OpenGLFunctions::glUniform4iv(UniformIndex, 1,
                                                      &Vector.X);

    } else if CONSTEXPR_IF (std::is_unsigned_v<
                                typename VectorType::ComponentType>) {
#ifdef SDL_PLATFORM_VITA
        Exceptions::ThrowException<Exceptions::RuntimeError>(
            "Function "
            "'Game::Graphics::Program::Wrappers::OpenGL::SetUniform_vec2<Math::"
            "UVector4>' is "
            "not "
            "implemented on this platform.");
#else
        Game::Graphics::OpenGLFunctions::glUniform4uiv(UniformIndex, 1,
                                                       &Vector.X);
#endif
    }
}

template void Game::Graphics::Program::Wrappers::OpenGL::SetUniform_vec4<
    Math::IVector4>(Sint32 UniformIndex, const Math::IVector4& BVector);

template void Game::Graphics::Program::Wrappers::OpenGL::SetUniform_vec4<
    Math::UVector4>(Sint32 UniformIndex, const Math::UVector4& BVector);

template void Game::Graphics::Program::Wrappers::OpenGL::SetUniform_vec4<
    Math::Vector4>(Sint32 UniformIndex, const Math::Vector4& BVector);

void Game::Graphics::Program::Wrappers::OpenGL::SetUniformMat2(
    Sint32 UniformIndex,
    const Math::Mat2& Mat2) {
    Game::Graphics::OpenGLFunctions::glUniformMatrix2fv(UniformIndex, 1,
                                                        GL_FALSE, &Mat2.R0.X);
}

void Game::Graphics::Program::Wrappers::OpenGL::SetUniformMat3(
    Sint32 UniformIndex,
    const Math::Mat3& Mat3) {
    Game::Graphics::OpenGLFunctions::glUniformMatrix3fv(UniformIndex, 1,
                                                        GL_FALSE, &Mat3.R0.X);
}

void Game::Graphics::Program::Wrappers::OpenGL::SetUniformMat4(
    Sint32 UniformIndex,
    const Math::Mat4& Mat4) {
    Game::Graphics::OpenGLFunctions::glUniformMatrix4fv(UniformIndex, 1,
                                                        GL_FALSE, &Mat4.R0.X);
}

Game::Graphics::Classes::Program*
    Game::Graphics::Program::Wrappers::OpenGL::Create() {
    Game::Graphics::Classes::Program_OpenGL* NewProgram =
        new Game::Graphics::Classes::Program_OpenGL;
    NewProgram->GLObject = Game::Graphics::OpenGLFunctions::glCreateProgram();

    return NewProgram;
}

Game::Graphics::Classes::Program_OpenGL::~Program_OpenGL() {
    if (this->GLObject == 0) {
        return;
    }
    Game::Graphics::OpenGLFunctions::glDeleteProgram(this->GLObject);
}

int Game::Graphics::Classes::Program_OpenGL::GetUniformLocation(
    const char* UniformName) const {
    return Game::Graphics::OpenGLFunctions::glGetUniformLocation(this->GLObject,
                                                                 UniformName);
}

bool Game::Graphics::Classes::Program_OpenGL::Link(bool Cache) {
    Game::Graphics::OpenGLFunctions::glLinkProgram(this->GLObject);
    return true;
}

void Game::Graphics::Classes::Program_OpenGL::Use() {
    Game::Graphics::OpenGLFunctions::glUseProgram(this->GLObject);
}