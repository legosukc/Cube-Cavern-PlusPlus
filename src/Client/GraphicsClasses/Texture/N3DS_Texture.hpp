#pragma once

#include "Base_Texture.hpp"

#include <citro3d.h>

namespace Game::Graphics::Classes {

    class Texture_N3DS : public Texture {
       public:
        virtual ~Texture_N3DS() override;

        C3D_Tex TexObj;

        virtual void Bind(int BindIndex) override;
        virtual bool IsBound() const override;
    };
}

namespace Game::Graphics::Texture::Wrappers {

    extern Game::Graphics::Classes::Texture_N3DS* BoundTexture_N3DS;

    Game::Graphics::Classes::Texture* Create_N3DS();

    void CreateBulk_N3DS(size_t CreateAmount,
                         Game::Graphics::Classes::Texture* Textures[]);

    void Unbind_N3DS();

    void UploadPixelData_N3DS(size_t Width,
                              size_t Height,
                              unsigned int Format,
                              unsigned int PixelFormat,
                              const void* PixelData);

    void SetFilteringMode_N3DS(unsigned int FilteringMode);

    void AttachToFramebuffer_N3DS(unsigned int AttachmentType,
                                  Game::Graphics::Classes::Texture* Texture);
}