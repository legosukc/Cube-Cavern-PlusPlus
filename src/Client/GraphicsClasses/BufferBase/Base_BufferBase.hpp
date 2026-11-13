#pragma once

namespace Game::Graphics::Classes {

    class BufferBase {
       public:
        virtual ~BufferBase();

        virtual void Bind();
        virtual bool IsBound() const;
    };
}