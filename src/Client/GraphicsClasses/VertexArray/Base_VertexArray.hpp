#pragma once

namespace Game::Graphics::Classes {

    class VertexArray {
       public:
        virtual ~VertexArray();

        virtual void Bind();
        virtual bool IsBound() const;
    };
}