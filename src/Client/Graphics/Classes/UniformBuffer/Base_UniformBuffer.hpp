#pragma once

#include "../BufferBase/Base_BufferBase.hpp"

namespace Game::Graphics::Classes {

    class UniformBuffer : virtual public Game::Graphics::Classes::BufferBase {
       protected:
        UniformBuffer();
        friend Game::Graphics::Classes::BufferBase;

       public:
        virtual void BindBase(GLuint Index);
    };
}