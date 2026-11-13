#pragma once

#include "Base_Program.hpp"

#ifdef SDL_PLATFORM_3DS
#include <citro3d.h>
#else
#error "Included '3DS_Program.hpp' in a non-N3DS build."
#endif

namespace Game::Graphics::Program::Wrappers::N3DS {

    Game::Graphics::Classes::Program* Create();

    void SetUniformSint32(Sint32 UniformIndex, Sint32 Value);
    void SetUniformUint32(Sint32 UniformIndex, Uint32 Value);
    void SetUniformBool(Sint32 UniformIndex, bool Value);
    void SetUniformFloat(Sint32 UniformIndex, float Value);

    template <class BoolVectorType>
    void SetUniformBvec_(Sint32 UniformIndex, const BoolVectorType& BVector);

    template <class VectorType>
    void SetUniform_vec2(Sint32 UniformIndex, const VectorType& Vector);

    template <class VectorType>
    void SetUniform_vec3(Sint32 UniformIndex, const VectorType& Vector);

    template <class VectorType>
    void SetUniform_vec4(Sint32 UniformIndex, const VectorType& Vector);

    void SetUniformMat2(Sint32 UniformIndex, const Math::Mat2& Mat2);

    void SetUniformMat3(Sint32 UniformIndex, const Math::Mat3& Mat3);

    void SetUniformMat4(Sint32 UniformIndex, const Math::Mat4& Mat4);
}

namespace Game::Graphics::Classes {

    class Program_N3DS : public Program {
       public:
        virtual ~Program_N3DS() override;

        shaderProgram_s N3DSProgramObject;

        virtual int GetUniformLocation(
            const char* UniformName) const override;

        virtual bool Link(bool Cache = true) override;
        virtual void Use() override;
    };
}