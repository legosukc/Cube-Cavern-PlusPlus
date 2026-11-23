#pragma once

#include "../BaseClassDefinitions.hpp"

namespace Game::Graphics::Classes {

    class Framebuffer : public Game::Graphics::Classes::OpenGLClassBase {
       public:
        virtual ~Framebuffer();

        virtual void Bind();
        virtual bool IsBound() const;
    };
}