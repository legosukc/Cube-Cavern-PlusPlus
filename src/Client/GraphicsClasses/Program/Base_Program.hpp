#pragma once

#include "../BaseClassDefinitions.hpp"

namespace Game::Graphics::Classes {

    class Program : public Game::Graphics::Classes::OpenGLClassBase {
       public:
        virtual ~Program();

        virtual int GetUniformLocation(const char* UniformName) const;

        virtual bool Link(bool Cache = true);
        virtual void Use();
    };
}