#pragma once

#include "../BaseClassDefinitions.hpp"

namespace Game::Graphics::Classes {

    class ShaderBase : public Game::Graphics::Classes::OpenGLClassBase {
       public:
        virtual ~ShaderBase();

        virtual bool SetShaderSource(const char* Source);
        virtual bool Compile();
    };
}