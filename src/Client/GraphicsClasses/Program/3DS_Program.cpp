#include "3DS_Program.hpp"

#include <SDL3/SDL_platform_defines.h>

#ifdef SDL_PLATFORM_3DS
#include <citro3d.h>
#else
#error "Trying to compile '3DS_Program.cpp' in a non-3DS build."
#endif

void Game::Graphics::Program::Wrappers::N3DS::SetUniformSint32(
    Sint32 UniformIndex,
    Sint32 Value) {
    //Game::Graphics::OpenGLFunctions::glUniform1i(UniformIndex, Value);
}

void Game::Graphics::Program::Wrappers::N3DS::SetUniformUint32(
    Sint32 UniformIndex,
    Uint32 Value) {

    Exceptions::ThrowException<Exceptions::RuntimeError>(
        "Function "
        "'Game::Graphics::Program::Wrappers::N3DS::SetUniformUint32' is not "
        "implemented on this platform.");
}

void Game::Graphics::Program::Wrappers::N3DS::SetUniformBool(
    Sint32 UniformIndex,
    bool Value) {
    *reinterpret_cast<Uint32>(C3D_IVUnifWritePtr(
        GPU_VERTEX_SHADER, UniformIndex)) = (Uint32)Value;
}

void Game::Graphics::Program::Wrappers::N3DS::SetUniformFloat(
    Sint32 UniformIndex,
    float Value) {
    *reinterpret_cast<Math::Vector4*>(C3D_FVUnifWritePtr(
        GPU_VERTEX_SHADER, UniformIndex)) = Math::Vector4(Value);
}

template <class BoolVectorType>
void Game::Graphics::Program::Wrappers::N3DS::SetUniformBvec_(
    Sint32 UniformIndex,
    const BoolVectorType& BVector) {
    /*
if CONSTEXPR_IF (BoolVectorType::ComponentCount == 2) {
    Game::Graphics::OpenGLFunctions::glUniform2iv(UniformIndex, 1,
                                                  (const GLint*)&BVector);
} else if CONSTEXPR_IF (BoolVectorType::ComponentCount == 3) {
    Game::Graphics::OpenGLFunctions::glUniform3iv(UniformIndex, 1,
                                                  (const GLint*)&BVector);
} else if CONSTEXPR_IF (BoolVectorType::ComponentCount == 4) {
    Game::Graphics::OpenGLFunctions::glUniform4iv(UniformIndex, 1,
                                                  (const GLint*)&BVector);
}*/
}

template void Game::Graphics::Program::Wrappers::N3DS::SetUniformBvec_<
    Math::IVector2>(Sint32 UniformIndex, const Math::IVector2& BVector);

template void Game::Graphics::Program::Wrappers::N3DS::SetUniformBvec_<
    Math::IVector3>(Sint32 UniformIndex, const Math::IVector3& BVector);

template void Game::Graphics::Program::Wrappers::N3DS::SetUniformBvec_<
    Math::IVector4>(Sint32 UniformIndex, const Math::IVector4& BVector);

template <class VectorType>
void Game::Graphics::Program::Wrappers::N3DS::SetUniform_vec2(
    Sint32 UniformIndex,
    const VectorType& Vector) {
    if CONSTEXPR_IF (std::is_floating_point_v<
                         typename VectorType::ComponentType>) {
        C3D_FVec* Uniform = C3D_FVUnifWritePtr(GPU_VERTEX_SHADER, UniformIndex);
        *reinterpret_cast<VectorType*>(Uniform) = Vector;
        Uniform[2] = 0.f;
        Uniform[3] = 0.f;

    } else if CONSTEXPR_IF (std::is_signed_v<
                                typename VectorType::ComponentType>) {
        C3D_IVUnifSet(GPU_VERTEX_SHADER, UniformIndex, Vector.X, Vector.Y, 0,
                      0);

    } else if CONSTEXPR_IF (std::is_unsigned_v<
                                typename VectorType::ComponentType>) {
        Exceptions::ThrowException<Exceptions::RuntimeError>(
            "Function "
            "'Game::Graphics::Program::Wrappers::N3DS::SetUniform_vec2<Math::"
            "UVector2>' is "
            "not "
            "implemented on this platform.");
    }
}

template void Game::Graphics::Program::Wrappers::N3DS::SetUniform_vec2<
    Math::IVector2>(Sint32 UniformIndex, const Math::IVector2& BVector);

template void Game::Graphics::Program::Wrappers::N3DS::SetUniform_vec2<
    Math::UVector2>(Sint32 UniformIndex, const Math::UVector2& BVector);

template void Game::Graphics::Program::Wrappers::N3DS::SetUniform_vec2<
    Math::Vector2>(Sint32 UniformIndex, const Math::Vector2& BVector);

template <class VectorType>
void Game::Graphics::Program::Wrappers::N3DS::SetUniform_vec3(
    Sint32 UniformIndex,
    const VectorType& Vector) {
    if CONSTEXPR_IF (std::is_floating_point_v<
                         typename VectorType::ComponentType>) {
        C3D_FVec* Uniform = C3D_FVUnifWritePtr(GPU_VERTEX_SHADER, UniformIndex);
        *reinterpret_cast<VectorType*>(Uniform) = Vector;
        Uniform[3] = 0.f;

    } else if CONSTEXPR_IF (std::is_signed_v<
                                typename VectorType::ComponentType>) {
        C3D_IVUnifSet(GPU_VERTEX_SHADER, UniformIndex, Vector.X, Vector.Y,
                      Vector.Z, 0);

    } else if CONSTEXPR_IF (std::is_unsigned_v<
                                typename VectorType::ComponentType>) {
        Exceptions::ThrowException<Exceptions::RuntimeError>(
            "Function "
            "'Game::Graphics::Program::Wrappers::N3DS::SetUniform_vec3<Math::"
            "UVector3>' is "
            "not "
            "implemented on this platform.");
    }
}

template void Game::Graphics::Program::Wrappers::N3DS::SetUniform_vec3<
    Math::IVector3>(Sint32 UniformIndex, const Math::IVector3& BVector);

template void Game::Graphics::Program::Wrappers::N3DS::SetUniform_vec3<
    Math::UVector3>(Sint32 UniformIndex, const Math::UVector3& BVector);

template void Game::Graphics::Program::Wrappers::N3DS::SetUniform_vec3<
    Math::Vector3>(Sint32 UniformIndex, const Math::Vector3& BVector);

template <class VectorType>
void Game::Graphics::Program::Wrappers::N3DS::SetUniform_vec4(
    Sint32 UniformIndex,
    const VectorType& Vector) {
    if CONSTEXPR_IF (std::is_floating_point_v<
                         typename VectorType::ComponentType>) {
        *C3D_FVUnifWritePtr(GPU_VERTEX_SHADER, UniformIndex) =
            *reinterpret_cast<C3D_FVec*>(&Vector);

    } else if CONSTEXPR_IF (std::is_signed_v<
                                typename VectorType::ComponentType>) {
        C3D_IVUnifSet(GPU_VERTEX_SHADER, UniformIndex, Vector.X, Vector.Y,
                      Vector.Z, Vector.W);

    } else if CONSTEXPR_IF (std::is_unsigned_v<
                                typename VectorType::ComponentType>) {
        Exceptions::ThrowException<Exceptions::RuntimeError>(
            "Function "
            "'Game::Graphics::Program::Wrappers::N3DS::SetUniform_vec4<Math::"
            "UVector4>' is "
            "not "
            "implemented on this platform.");
    }
}

template void Game::Graphics::Program::Wrappers::N3DS::SetUniform_vec4<
    Math::IVector4>(Sint32 UniformIndex, const Math::IVector4& BVector);

template void Game::Graphics::Program::Wrappers::N3DS::SetUniform_vec4<
    Math::UVector4>(Sint32 UniformIndex, const Math::UVector4& BVector);

template void Game::Graphics::Program::Wrappers::N3DS::SetUniform_vec4<
    Math::Vector4>(Sint32 UniformIndex, const Math::Vector4& BVector);

void Game::Graphics::Program::Wrappers::N3DS::SetUniformMat2(
    Sint32 UniformIndex,
    const Math::Mat2& Mat2) {
        /*
    Game::Graphics::OpenGLFunctions::glUniformMatrix2fv(UniformIndex, 1,
                                                        GL_FALSE, &Mat2.R0.X);
                                                        */
}

void Game::Graphics::Program::Wrappers::N3DS::SetUniformMat3(
    Sint32 UniformIndex,
    const Math::Mat3& Mat3) {
        /*
    Game::Graphics::OpenGLFunctions::glUniformMatrix3fv(UniformIndex, 1,
                                                        GL_FALSE, &Mat3.R0.X);
                                                        */
}

void Game::Graphics::Program::Wrappers::N3DS::SetUniformMat4(
    Sint32 UniformIndex,
    const Math::Mat4& Mat4) {
    C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER, UniformIndex,
                     reinterpret_cast<const C3D_Mtx*>(&Mat4));
}

Game::Graphics::Classes::Program*
    Game::Graphics::Program::Wrappers::N3DS::Create() {

    Game::Graphics::Classes::Program_N3DS* NewProgram =
        new Game::Graphics::Classes::Program_N3DS;

    shaderProgramInit(&NewProgram->N3DSProgramObject);

    return NewProgram;
}

Game::Graphics::Classes::Program_OpenGL::~Program_OpenGL() {
    shaderProgramFree(&this->N3DSProgramObject);
}

int Game::Graphics::Classes::Program_OpenGL::GetUniformLocation(
    const char* UniformName) const {
    return shaderInstanceGetUniformLocation(
        this->N3DSProgramObject.vertexShader, Game::Graphics::UniformBuffer);
}

bool Game::Graphics::Classes::Program_OpenGL::Link(bool Cache) {
    // Game::Graphics::OpenGLFunctions::glLinkProgram(this->GLObject);
    return true;
}

void Game::Graphics::Classes::Program_OpenGL::Use() {
    C3D_BindProgram(&this->N3DSProgramObject);
}