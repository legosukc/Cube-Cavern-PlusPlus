
#include "../../define.h"

#include "Program.hpp"

#include <SDL3/SDL_opengl.h>
#include <SDL3/SDL_opengl_glext.h>
#include <SDL3/SDL_video.h>

#include "../../MathClasses/Matrix.hpp"
#include "../../MathClasses/Vector2.hpp"
#include "../../MathClasses/Vector3.hpp"
#include "../../MathClasses/Vector4.hpp"

namespace Game::Graphics::OpenGLFunctions {

    PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
    PFNGLGETUNIFORMBLOCKINDEXPROC glGetUniformBlockIndex;
    PFNGLUNIFORMBLOCKBINDINGPROC glUniformBlockBinding;

    PFNGLATTACHSHADERPROC glAttachShader;
    PFNGLGETATTACHEDSHADERSPROC glGetAttachedShaders;
    PFNGLDETACHSHADERPROC glDetachShader;

    PFNGLLINKPROGRAMPROC glLinkProgram;
    PFNGLUSEPROGRAMPROC glUseProgram;

    PFNGLGETPROGRAMIVPROC glGetProgramiv;

    PFNGLUNIFORM1IPROC glUniform1i;
    PFNGLUNIFORM1UIPROC glUniform1ui;
    PFNGLUNIFORM1FPROC glUniform1f;

    PFNGLUNIFORM2IVPROC glUniform1iv;
    PFNGLUNIFORM2UIVPROC glUniform1uiv;
    PFNGLUNIFORM2FVPROC glUniform1fv;

    PFNGLUNIFORMMATRIX2FVPROC glUniformMatrix2fv;
    PFNGLUNIFORMMATRIX3FVPROC glUniformMatrix3fv;
    PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv;
}

Game::Graphics::Classes::Program::~Program() = default;

GLint Game::Graphics::Classes::Program::GetUniformLocation(
    const char* UniformName) const {
    return -1;
}

bool Game::Graphics::Classes::Program::Link() {
    return false;
}
void Game::Graphics::Classes::Program::Use() {}

Game::Graphics::Classes::Program_OpenGL::~Program_OpenGL() {
    if (this->GLObject == 0) {
        return;
    }
    Game::Graphics::OpenGLFunctions::glDeleteProgram(this->GLObject);
}

GLint Game::Graphics::Classes::Program_OpenGL::GetUniformLocation(
    const char* UniformName) const {
    return Game::Graphics::OpenGLFunctions::glGetUniformLocation(this->GLObject,
                                                                 UniformName);
}

bool Game::Graphics::Classes::Program_OpenGL::Link() {
    Game::Graphics::OpenGLFunctions::glLinkProgram(this->GLObject);
    return true;
}

void Game::Graphics::Classes::Program_OpenGL::Use() {
    Game::Graphics::OpenGLFunctions::glUseProgram(this->GLObject);
}

Game::Graphics::Classes::Program
    Game::Graphics::Program::Wrappers::OpenGL::Create() {
    Game::Graphics::Classes::Program_OpenGL NewProgram;
    NewProgram.GLObject = Game::Graphics::OpenGLFunctions::glCreateProgram();

    return NewProgram;
}

void Game::Graphics::Program::Wrappers::OpenGL::SetUniformSint32(
    Sint32 UniformIndex,
    Sint32 Value) {
    Game::Graphics::OpenGLFunctions::glUniform1i(UniformIndex, Value);
}

void Game::Graphics::Program::Wrappers::OpenGL::SetUniformUint32(
    Sint32 UniformIndex,
    Uint32 Value) {
    Game::Graphics::OpenGLFunctions::glUniform1ui(UniformIndex, Value);
}

void Game::Graphics::Program::Wrappers::OpenGL::SetUniformBool(
    Sint32 UniformIndex,
    bool Value) {
    Game::Graphics::OpenGLFunctions::glUniform1ui(UniformIndex, Value);
}

void Game::Graphics::Program::Wrappers::OpenGL::SetUniformFloat(
    Sint32 UniformIndex,
    float Value) {
    Game::Graphics::OpenGLFunctions::glUniform1f(UniformIndex, Value);
}

template <class BoolVectorType>
void Game::Graphics::Program::Wrappers::OpenGL::SetUniformBvec_(
    Sint32 UniformIndex,
    const BoolVectorType BVector) {
    Game::Graphics::OpenGLFunctions::glUniform1iv(
        UniformIndex, sizeof(BoolVectorType), &BVector);
}

template <class VectorType>
void Game::Graphics::Program::Wrappers::OpenGL::SetUniform_vec_(
    Sint32 UniformIndex,
    const VectorType& Vector) {
    using namespace Game::Graphics::OpenGLFunctions;

    if constexpr (std::is_floating_point_v<
                      typename VectorType::ComponentType>) {
        glUniform1fv(UniformIndex, VectorType::ComponentCount, &Vector.X);

    } else if constexpr (std::is_signed_v<typename VectorType::ComponentType>) {
        glUniform1iv(UniformIndex, VectorType::ComponentCount, &Vector.X);

    } else if constexpr (std::is_unsigned_v<
                             typename VectorType::ComponentType>) {
        glUniform1uiv(UniformIndex, VectorType::ComponentCount, &Vector.X);
    }
}

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

// TODO: make the SetUniformBvec_ functions use a Bvec_ struct.

namespace Game::Graphics::Program {

    Game::Graphics::Classes::Program (*Create)();

    void (*SetUniformSint32)(Sint32 UniformIndex, Sint32 Value);
    void (*SetUniformUint32)(Sint32 UniformIndex, Uint32 Value);
    void (*SetUniformBool)(Sint32 UniformIndex, bool Value);
    void (*SetUniformFloat)(Sint32 UniformIndex, float Value);

    void (*SetUniformIvec2)(Sint32 UniformIndex, const Math::IVector2& Ivec2);
    void (*SetUniformUvec2)(Sint32 UniformIndex, const Math::UVector2& Uvec2);
    void (*SetUniformBvec2)(Sint32 UniformIndex, const Math::IVector2& Bvec2);
    void (*SetUniformFvec2)(Sint32 UniformIndex, const Math::Vector2& Fvec2);

    void (*SetUniformIvec3)(Sint32 UniformIndex, const Math::IVector3& Ivec3);
    void (*SetUniformUvec3)(Sint32 UniformIndex, const Math::UVector3& Uvec3);
    void (*SetUniformBvec3)(Sint32 UniformIndex, const Math::IVector3& Bvec3);
    void (*SetUniformFvec3)(Sint32 UniformIndex, const Math::Vector3& Fvec3);

    void (*SetUniformIvec4)(Sint32 UniformIndex, const Math::IVector4& Ivec4);
    void (*SetUniformUvec4)(Sint32 UniformIndex, const Math::UVector4& Uvec4);
    void (*SetUniformBvec4)(Sint32 UniformIndex, const Math::IVector4& Bvec4);
    void (*SetUniformFvec4)(Sint32 UniformIndex, const Math::Vector4& Fvec4);

    void (*SetUniformMat2)(Sint32 UniformIndex, const Math::Mat2& Mat2);
    void (*SetUniformMat3)(Sint32 UniformIndex, const Math::Mat3& Mat3);
    void (*SetUniformMat4)(Sint32 UniformIndex, const Math::Mat4& Mat4);
}

void Game::Graphics::Program::Init_OpenGL() {
    LOAD_OPENGL_FUNCTION(glCreateProgram);
    LOAD_OPENGL_FUNCTION(glDeleteProgram);

    LOAD_OPENGL_FUNCTION(glGetUniformLocation);
    LOAD_OPENGL_FUNCTION(glGetUniformBlockIndex);

    LOAD_OPENGL_FUNCTION(glUniformBlockBinding);

    LOAD_OPENGL_FUNCTION(glAttachShader);
    LOAD_OPENGL_FUNCTION(glGetAttachedShaders);
    LOAD_OPENGL_FUNCTION(glDetachShader);

    LOAD_OPENGL_FUNCTION(glLinkProgram);
    LOAD_OPENGL_FUNCTION(glUseProgram);

    LOAD_OPENGL_FUNCTION(glGetProgramiv);

    LOAD_OPENGL_FUNCTION(glUniform1i);
    LOAD_OPENGL_FUNCTION(glUniform1ui);
    LOAD_OPENGL_FUNCTION(glUniform1f);

    LOAD_OPENGL_FUNCTION(glUniformMatrix2fv);
    LOAD_OPENGL_FUNCTION(glUniformMatrix3fv);
    LOAD_OPENGL_FUNCTION(glUniformMatrix4fv);

    Program::Create = Game::Graphics::Program::Wrappers::OpenGL::Create;

    Program::SetUniformSint32 =
        Game::Graphics::Program::Wrappers::OpenGL::SetUniformSint32;
    Program::SetUniformUint32 =
        Game::Graphics::Program::Wrappers::OpenGL::SetUniformUint32;
    Program::SetUniformBool =
        Game::Graphics::Program::Wrappers::OpenGL::SetUniformBool;
    Program::SetUniformFloat =
        Game::Graphics::Program::Wrappers::OpenGL::SetUniformFloat;

    Program::SetUniformIvec2 =
        Game::Graphics::Program::Wrappers::OpenGL::SetUniform_vec_<
            Math::IVector2>;
    Program::SetUniformUvec2 =
        Game::Graphics::Program::Wrappers::OpenGL::SetUniform_vec_<
            Math::UVector2>;
    Program::SetUniformBvec2 =
        Game::Graphics::Program::Wrappers::OpenGL::SetUniform_vec_<
            Math::IVector2>;
    Program::SetUniformFvec2 =
        Game::Graphics::Program::Wrappers::OpenGL::SetUniform_vec_<
            Math::Vector2>;

    Program::SetUniformIvec3 =
        Game::Graphics::Program::Wrappers::OpenGL::SetUniform_vec_<
            Math::IVector3>;
    Program::SetUniformUvec3 =
        Game::Graphics::Program::Wrappers::OpenGL::SetUniform_vec_<
            Math::UVector3>;
    Program::SetUniformBvec3 =
        Game::Graphics::Program::Wrappers::OpenGL::SetUniform_vec_<
            Math::IVector3>;
    Program::SetUniformFvec3 =
        Game::Graphics::Program::Wrappers::OpenGL::SetUniform_vec_<
            Math::Vector3>;

    Program::SetUniformIvec4 =
        Game::Graphics::Program::Wrappers::OpenGL::SetUniform_vec_<
            Math::IVector4>;
    Program::SetUniformUvec4 =
        Game::Graphics::Program::Wrappers::OpenGL::SetUniform_vec_<
            Math::UVector4>;
    Program::SetUniformBvec4 =
        Game::Graphics::Program::Wrappers::OpenGL::SetUniform_vec_<
            Math::IVector4>;
    Program::SetUniformFvec4 =
        Game::Graphics::Program::Wrappers::OpenGL::SetUniform_vec_<
            Math::Vector4>;

    Program::SetUniformMat2 =
        Game::Graphics::Program::Wrappers::OpenGL::SetUniformMat2;
    Program::SetUniformMat3 =
        Game::Graphics::Program::Wrappers::OpenGL::SetUniformMat3;
    Program::SetUniformMat4 =
        Game::Graphics::Program::Wrappers::OpenGL::SetUniformMat4;
}

void Game::Graphics::Program::Init_Vulkan() {}

void Game::Graphics::Program::Init_Metal() {}

void Game::Graphics::Program::Init_DirectX11() {}

void Game::Graphics::Program::Init_DirectX12() {}