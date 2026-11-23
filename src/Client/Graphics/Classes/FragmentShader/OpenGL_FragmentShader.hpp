#pragma once

#include "../../../../define.h"

#include "../ShaderBase/OpenGL_ShaderBase.hpp"

#include "../FragmentShader.hpp"

namespace Game::Graphics::Classes {

    class FragmentShader_OpenGL
        : public Game::Graphics::Classes::ShaderBase_OpenGL<
              GL_FRAGMENT_SHADER> {
       public:
        virtual bool SetShaderSource(const char* Source) override;
        virtual bool Compile() override;
    };
}

namespace Game::Graphics::FragmentShader::OpenGL {
    inline void Init() {
        using namespace Game::Graphics::ShaderBase;
        using FragmentShader = Game::Graphics::Classes::FragmentShader;
        using FragmentShader_OpenGL =
            Game::Graphics::Classes::FragmentShader_OpenGL;

        Game::Graphics::FragmentShader::Create =
            WrapperTemplates::Create_OpenGL<FragmentShader,
                                            FragmentShader_OpenGL>;
        Game::Graphics::FragmentShader::CreateBulk =
            WrapperTemplates::CreateBulk_OpenGL<FragmentShader,
                                                FragmentShader_OpenGL>;
    }
}