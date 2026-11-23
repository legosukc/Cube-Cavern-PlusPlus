#pragma once

namespace Game::Graphics::Classes {

    class Texture {
       public:
        virtual ~Texture();

        virtual void Bind(int BindIndex);
        virtual bool IsBound() const;
    };
}